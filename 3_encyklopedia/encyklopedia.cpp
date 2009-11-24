#include <iostream>
#include <algorithm>

typedef long Integer;

const Integer INFTY = 100000;

int main() {
  
  Integer perm_length;
  Integer *perm, *min_ends;
  Integer max_length = 1;
  
  /* wczytaj długość permutacji; */
  std::cin >> perm_length;
  
  /* wczytaj permutację; */
  perm = new Integer[perm_length];
  for(Integer i = 0; i < perm_length; i ++)
    std::cin >> perm[i];
  
  min_ends = new Integer[perm_length];
  for(Integer i = 0; i < perm_length; i ++)
    min_ends[i] = INFTY;
  min_ends[0] = perm[0]; /* [0..perm_length-1]; s*/
  /* najmniejsza liczba, ktorą kończy się podciąg
     rosnący długości l + 1; */
  
  for(Integer i = 1; i < perm_length; i ++) {
    /* i - indeks w permutacji; */
    Integer *ptr = std::lower_bound(min_ends, min_ends + perm_length, perm[i]);
    Integer index = ptr - min_ends;
    min_ends[index] = perm[i];
    max_length = ((index + 1) > max_length) ? (index + 1) : max_length;
    /*for(Integer j = 0; j <= i; j ++) {
      if(min_ends[j] == INFTY) {
        min_ends[j] = i;
        max_length = ((j + 1) > max_length) ? (j + 1) : max_length;
        std::cerr << "len=" << j + 1 << "; i=" << i << "; end" << std::endl;
        break;
      }
      if(perm[min_ends[j]] > perm[i]) {
        min_ends[j] = i;
        max_length = ((j + 1) > max_length) ? (j + 1) : max_length;
        std::cerr << "len=" << j + 1 << "; i=" << i << "; mid" << std::endl;
        break;
      }
    }*/
  }
  
  //for(Integer i = 1; i <= perm_length; i++)
  //  std::cerr << "l = " << i << ": " << min_ends[i] << std::endl;
  
  /* wypisz wynik na stdout; */
  std::cout << (perm_length - max_length) << std::flush;
  
  return 0;
}
