/** ASD 2009, Laboratorium;
 ** Zadanie 4: Procenty;
 ** Cezary Bartoszuk, cb277617, <cbart@students.mimuw.edu.pl>
 **/

#include <iostream>
#include <string>
#include <algorithm>

/** typ całkowitoliczbowy używany do indeksowania sklepów; */
typedef int IntCounter;

/** typ całkowitoliczbowy używany do pomiaru czasu (w minutach); */
typedef long long IntTime;

/** typ całkowitoliczbowy używany do pomiaru odległości (w km); */
typedef long long IntDistance;

/** sklep;
 ** zna swoją minutę zamknięcia i odległość od N-końca ulicy; */
struct Shop {
  public:
    IntTime closing_time;
    IntDistance position;
};

#ifdef DEBUG
const unsigned int DEBUG_LEVEL = 1;
#else
const unsigned int DEBUG_LEVEL = 0;
#endif

const IntTime INFTY_TIME = -1;

const std::string ANSWER_NO = "NIE";

/** "sprytne" minimum, dla którego wartość INFTY_TIME
 ** jest większa lub równa niż wszystkie inne wartości; */
inline IntTime min(IntTime t1, IntTime t2) {
  if(t1 == INFTY_TIME)
    return t2;
  else
    if(t2 == INFTY_TIME)
      return t1;
    else
      return (t1 < t2) ? t1 : t2;
}

/** funkcja porównująca dwa sklepy względem położenia; */
bool compareShopsWithDistance(Shop s1, Shop s2) {
  return (s1.position < s2.position);
}

int main() {
  IntCounter n;    /* liczba sklepów; */
  Shop* shops;     /* tablica sklepów; */
  IntTime** paths; /* tablica minimalnych czasów ukończenia ścieżki; */

  /* optymalizacja: wyłączam synchronizację z stdio; */
  if(DEBUG_LEVEL == 0)
    std::ios_base::sync_with_stdio(0);

  /* wczytuję liczbę sklepów; */
  std::cin >> n;

  /* DEBUG CODE: */
  if(DEBUG_LEVEL > 0) {
    std::cerr << "len=" << n << ";\n";
    std::cerr << std::flush;
  } /* DEBUG_LEVEL > 0; */

  /* alokuję pamięć na tablicę sklepów; */
  shops = new Shop[n];

  /* wczytuję sklepy ze standardowego wejścia; */
  for(IntCounter i = 0; i < n; i ++) {
    std::cin >> shops[i].position;
    std::cin >> shops[i].closing_time;
  }

  /* DEBUG CODE: */
  if(DEBUG_LEVEL > 0) {
    for(IntCounter i = 0; i < n; i ++) {
      std::cerr << "[" << i << "]: pos=" << shops[i].position
                << ", time=" << shops[i].closing_time << ";\n";
    }
    std::cerr << std::flush;
  } /* DEBUG_LEVEL > 0; */

  /** algorytm:
   ** zał: sklepy posortowane względem odległości od N-końca ulicy;
   ** pomysł opiera się na fakcie, że jeżeli chcemy obliczyć najkrótszą
   ** ścieżkę z [i->j] to albo musimy z końca ścieżki [i->j-1] dojść do
   ** [j] albo z końca ścieżki [j-1->i] dojść do [j]. Innej opcji
   ** nie ma, gdyż zawsze, jeżeli pokonaliśmy ścieżkę [i->j] to opłaca
   ** nam się iść dalej albo do [j+1] albo do [i-1].

   ** tablicować będziemy dla przejścia [i->j] następujące dane:
   ** -> minimalny czas ukończenia ścieżki
   ** (jeżeli takiej ścieżki nie da się pokonać to -1);

   ** w dalszych komentarzach "krokiem prostym" będę nazywał
   ** przypadek, gdy rozpatrujemy ścieżkę obejmującą sklepy [i..j]
   ** i przedłużającą się do sąsiedniego sklepu, czyli np. taką,
   ** kończy się w [j] i skacze dalej do [j+1];
   ** "krokiem na opak" zaś nazwę przypadek, gdy aby przedłużyć ścieżkę
   ** będzie trzeba "przejechać skuterem obok wszystkich odwiedzonych
   ** już w pierwotnej ścieżce sklepów", czyli np. gdy ścieżka
   ** obejmująca sklepy [i..j] i kończąca się w [j] skacze później
   ** do sklepu [i-1];
   ** Przypomnienie: sklepy będą posortowane względem długości
   ** od N-końca ulicy;
   **/

  /* 1. sortuję sklepy po odległościach; */

  /* DEBUG CODE: */
  if(DEBUG_LEVEL > 0) {
    std::cerr << "\nsort(shops)...\n";
    std::cerr << std::flush;
  } /* DEBUG_LEVEL > 0; */

  std::sort(shops, shops+n, compareShopsWithDistance);

  /* DEBUG CODE: */
  if(DEBUG_LEVEL > 0) {
    for(IntCounter i = 0; i < n; i ++) {
      std::cerr << "[" << i << "]: pos=" << shops[i].position
                << ", time=" << shops[i].closing_time << ";\n";
    }
    std::cerr << std::flush;
  } /* DEBUG_LEVEL > 0; */

  /* alokuję tablicę minimalnych czasów ukończenia ścieżki
   * od razu wypełniając zerami minimalne przejścia i->i; */
  paths = new IntTime*[n];
  for(IntCounter i = 0; i < n; i ++) {
    paths[i] = new IntTime[n];
    paths[i][i] = 0;
  }

  /* 2. dynamicznie wyliczam długości ścieżek; */

  /* DEBUG CODE: */
  if(DEBUG_LEVEL > 0) {
    std::cerr << "\nDynamic algorithm:\n\n" << std::flush;
  } /* DEBUG_LEVEL > 0; */

  for(IntCounter i = 1; i < n; i ++) {
    /* i - długość rozpatrywanego przedziału; */

    for(IntCounter j = 0; j < n - i; j ++) {
      /* j - indeks początkowy przedziału; */

      IntDistance distance;

      /* rozpatrujemy przedział [j.j+i] ścieżka kończy się na [j+i]; */

      /* DEBUG CODE: */
      if(DEBUG_LEVEL > 0) {
        std::cerr << j << "->" << j + i << ": ";
      } /* DEBUG_LEVEL > 0; */

      paths[j][j+i] = INFTY_TIME;

      /* [j->j+i-1] -> [j+i]; krok prosty; */

      if(paths[j][j+i-1] != INFTY_TIME) {
        distance = shops[j+i].position - shops[j+i-1].position;
        if(distance + paths[j][j+i-1] <= shops[j+i].closing_time)
          paths[j][j+i] = distance + paths[j][j+i-1];
      }

      /* [j+i-1->j] -> [j+i]; krok na opak; */

      if(paths[j+i-1][j] != INFTY_TIME) {
        distance = shops[j+i].position - shops[j].position;
        if(distance + paths[j+i-1][j] <= shops[j+i].closing_time)
          paths[j][j+i] =
            min(paths[j][j+i], distance + paths[j+i-1][j]);
      }


      /* DEBUG CODE: */
      if(DEBUG_LEVEL > 0) {
        std::cerr << paths[j][j+i] << "\n";
      } /* DEBUG_LEVEL > 0; */


      /* rozpatrujemy przedział [j..j+i] ścieżka kończy na [j]; */

      /* DEBUG CODE: */
      if(DEBUG_LEVEL > 0) {
        std::cerr << j + i << "->" << j << ": ";
      } /* DEBUG_LEVEL > 0; */

      paths[j+i][j] = INFTY_TIME;

      /* [j+i->j+1] -> [j]; krok prosty; */

      if(paths[j+i][j+1] != INFTY_TIME) {
        distance = shops[j+1].position - shops[j].position;
        if(distance + paths[j+i][j+1] <= shops[j].closing_time)
          paths[j+i][j] = distance + paths[j+i][j+1];
      }

      /* [j+1->j+i] -> [j]; krok na opak; */

      if(paths[j+1][j+i] != INFTY_TIME) {
        distance = shops[j+i].position - shops[j].position;
        if(distance + paths[j+1][j+i] <= shops[j].closing_time)
          paths[j+i][j] =
            min(paths[j+i][j], distance + paths[j+1][j+i]);
      }

      /* DEBUG CODE: */
      if(DEBUG_LEVEL > 0) {
        std::cerr << paths[j+i][j] << "\n";
      } /* DEBUG_LEVEL > 0; */
    }
  }

  /* DEBUG CODE: */
  if(DEBUG_LEVEL > 0)
    std::cerr << "\n" << std::flush;

  /* sprawdzam wynik; */
  IntTime min_time = min(paths[0][n-1], paths[n-1][0]);

  /* wypisuję wynik na stdout; */
  if(min_time == INFTY_TIME)
    std::cout << "NIE";
  else
    std::cout << min_time;

  /* opróżniam strumień wyjściowy; */
  std::cout << std::flush;

  /* sprzątam: */
  for(IntCounter i = 0; i < n; i ++)
    delete[] paths[i];
  delete[] paths;
  delete[] shops;

  return 0;
}

