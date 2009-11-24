#include <iostream>
#include <cmath>
#include <cassert>

typedef long Integer;

typedef long long LongInteger;

#ifdef DEBUG
const Integer DEBUG_LEVEL = 1;
#else
const Integer DEBUG_LEVEL = 0;
#endif

inline Integer lg2fl(Integer n) {
  return static_cast<Integer>(floor(log(static_cast<double>(n)) / log(2.0)));
}

/** drzewo przedziałowe [1..N] dla IntervalSet(N); */
class IntervalSet {
  private:
    Integer** tree;   /** poziom i indeks od lewej; */
    Integer length;   /** długość ostatniego poziomu; */
    Integer height;   /** wysokość drzewa = indeks ostatniego poziomu + 1; */
    Integer* lengths; /** długości tablic na poszczególnych poziomach:     *
                       ** 0 - korzeń;                                      */

  public:
    inline Integer minIntervalIndex(Integer lv_index, Integer lv);
    IntervalSet(Integer length); /** creates empty set; */
    virtual ~IntervalSet();      /** destroys set; */
    void insert(Integer number); /** inserts a number into a set; */
    LongInteger intervalCardinality(Integer from, Integer to);
      /** returns cardinality (quantum of elements) *
       ** of given interval (from, to - inclusive); */
};

IntervalSet::IntervalSet(Integer length_) : length(length_) {
  Integer max_index = length - 1; /* indeks ostatniego elementu w tablicy; */
  if(DEBUG_LEVEL > 0)
    std::cerr << "max_index = " << max_index << '\n';
  height = (lg2fl(max_index) + 1) + 1; /* wysokość drzewa; */
  /* alokuję tablicę */
  tree = new Integer*[height];
  lengths = new Integer[height];
  for(Integer level = height - 1, lv_max_index = max_index;
      level >= 0;
      level --, lv_max_index /= 2) {
    /* alokuję pamięć na dany poziom i wypełniam zerami (zbiór jest pusty); */
    tree[level] = new Integer[lv_max_index + 1];
    for(Integer i = 0; i <= lv_max_index; i ++)
      tree[level][i] = 0;
    lengths[level] = lv_max_index + 1;
    if(DEBUG_LEVEL > 0)
      std::cerr << "lv[" << level << "] = " << lv_max_index + 1 << '\n';
  }

  if(DEBUG_LEVEL > 0) {
    std::cerr << "Tree indexes:\n";
    for(Integer i = 0; i < height; i ++) {
      for(Integer j = 0; j < lengths[i]; j ++) {
        std::cerr << j << ' '; 
      }
      std::cerr << '\n';
    }
    std::cerr << '\n' << std::flush;
  }
}

IntervalSet::~IntervalSet() {
  /* czyszczę, co nabrudziłem; */
  for(Integer i = 0; i < height; i ++)
    delete[] tree[i];
  delete[] tree;
  delete[] lengths;
}

void IntervalSet::insert(Integer number) {
  /* wstawianie elementu:                  *
   * idziemy na ścieżce od danego elementu *
   * na najniższym poziomie aż do korzenia *
   * -> O(lgN);                            */
  for(Integer level = height - 1, lv_index = number - 1;
      level >= 0;
      level --, lv_index /= 2)
    tree[level][lv_index] ++;
}

inline Integer IntervalSet::minIntervalIndex(Integer lv_index, Integer lv) {
  /* zwraca indeks najmniejszego elementu w poddrzewie zdefiniowanym
   * przez podany poziom drzewa i indeks węzła (na tym poziomie); */
  return lv_index * (1 << (height - 1 - lv));
}

LongInteger IntervalSet::intervalCardinality(Integer from, Integer to) {
  LongInteger counter = 0;
  /* sprowadzam do inkeksów; */
  from --;
  to --;
  
  while(from <= to) {
    /* iteruję po [to]
     * - jeżeli krok iteracji w górę drzewa jest w prawo:
     *   - dodaję obecną pozycję do sumy i to -= 2^i
     *     gdzie i - ile kroków przeszedłem;
     * - jeżeli zaś krok iteracji podąża w lewo:
     *   - sprawdzam czy następny przedział się mieści:
     *     - jeżeli tak to przechodzę wyżej,
     *     - jeżeli nie to dodaję obecny i to -= 2^i
     *       gdzie i j.w.;
     */
     
    Integer index = to;
    Integer level = height - 1;
    while(index % 2 == 1 && level >= 0) {
      if(from <= minIntervalIndex(index / 2, level - 1)) {
        level --;
        index /= 2;
      }
      else
        break;
    }
    counter += tree[level][index];
    to = minIntervalIndex(index, level) - 1;
  }
  
  return counter;
}

int main() {
  Integer length;
  Integer perm_tmp_el;
  LongInteger transposes = 0;

  std::ios_base::sync_with_stdio(0);

  std::cin >> length;
  
  IntervalSet iset = IntervalSet(length);
  
  for(Integer i = 0; i < length; i ++) {
    if(DEBUG_LEVEL > 0)
      std::cerr << "<" << i << ">=";
    std::cin >> perm_tmp_el;
    transposes += iset.intervalCardinality(perm_tmp_el, length);
    if(DEBUG_LEVEL > 0)
      std::cerr << "#[" << perm_tmp_el << ", " << length << "] = "
                << iset.intervalCardinality(perm_tmp_el, length) << ";\n"
                << "inserting [" << perm_tmp_el << "];\n\n";
    iset.insert(perm_tmp_el);
  }
  if(DEBUG_LEVEL > 0)
    std::cerr << std::flush;

  std::cout << transposes << std::flush;

  return 0;
}
