#include <iostream>

using namespace std;

typedef long IntCount;
typedef long IntMod;
typedef long long IntSeq;
typedef long long IntSeqCount;

/* wylicza ciąg sum prefiksowych; */
IntSeq* init_sumuj_prefiksy_mod(IntSeq *ciag, IntCount n, IntMod mod) {
  /* deklaracje; */
  IntSeq *sumy = new IntSeq[n]; /* wynik - ciąg sum częściowych mod m; */
  IntSeq suma = 0;              /* wartość sumy "jak dotąd" w pętli; */
  
  for(int i = 0; i < n; i++) {
    suma += ciag[i];
    suma %= mod;
    sumy[i] = suma;
  }
  
  return sumy;
}

int main() {
  /* deklaracje; */
  IntCount n;                     /* liczba wyrazów ciągu; */
  IntMod m;                       /* moduł; */
  IntSeq *ciag, *sumy_prefiksowe; /* ciąg, na którym operujemy i sumy; */
  IntMod *prefiksy_mod_m;         /* prefiksy z potrzebnymi modułami; */
  IntSeqCount wynik = 0;          /* liczba podciągów; */

  /* wyłączam synchronizację z stdio; */
  ios_base::sync_with_stdio(0);
  
  /* wczytuję długość ciągu; */
  cin >> n;
  
  /* wczytuję moduł; */
  cin >> m;
  
  /* alokacja pamięci na ciąg sum; */
  ciag = new IntSeq[n];    
  
  /* wczytanie ciągu z wejścia; */
  for(IntCount i = 0; i < n; i ++)
      cin >> ciag[i];
  
  //cerr << "Mod: " << m << endl;
  //cerr << "Ciąg:\t\t\t";
  //for(int i = 0; i < n; i ++)
  //  cerr << "[" << i << "]=" << ciag[i] << " ";
  //cerr << endl;
  
  /* liczę ciąg sum częściowych; */
  sumy_prefiksowe = init_sumuj_prefiksy_mod(ciag, n, m);
  
  //cerr << "Sumy prefiksowe:\t";
  //for(int i = 0; i < n; i ++)
  //  cerr << "[" << i << "]=" << sumy_prefiksowe[i] << " ";
  //cerr << endl;
  
  /* alokacja pamięci na prefiksy; */
  prefiksy_mod_m = new IntMod[m];
  
  /* alokuję zerami; */
  for(IntMod i = 0; i < m; i ++)
    prefiksy_mod_m[i] = 0;
  
  /* uwzględniamy ciąg zerowy; */
  prefiksy_mod_m[0] = 1;
  
  /* wyliczam; */
  /* uwzględniamy ciąg zerowy; */
  wynik ++;
  for(IntCount i = 0; i < n; i ++)
    wynik += (prefiksy_mod_m[sumy_prefiksowe[i]] ++);
  
  /* wypisuję wynik na stdout; */
  cout << wynik;
  
  /* czyszczenie; */
  delete[] ciag;
  delete[] sumy_prefiksowe;
  delete[] prefiksy_mod_m;
  
  return 0;
}
