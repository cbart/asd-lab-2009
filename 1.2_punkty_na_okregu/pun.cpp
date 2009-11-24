/* Zadanie 1: Punkty na okrÄ�gu. */
/* Cezary Bartoszuk, cb277617@students.mimuw.edu.pl */

#include <iostream>                         /* strumienie I/O; */
#include <iomanip>                          /* do wypisania wyniku; */

using namespace std;

typedef long IntCounter;                    /* n: [1..10^6]; */
typedef long double IntLength;              /* suma: [1..10^9]; */

int main() {
    
    ios_base::sync_with_stdio(0);           /* wzglÄ�dy wydajnoĹ�ciowe; */
    
    IntCounter k, l;                        /* wskaĹşniki punktĂłw na okrÄ�gu; */
    IntCounter n;                           /* iloĹ�Ä� punktĂłw na okrÄ�gu; */
    IntLength *dlugosci;                    /* tablica dĹ�ugoĹ�ci; */
    IntLength d2_okregu = 0;                /* pĂłĹ� dĹ�ugoĹ�ci okrÄ�gu; */
    IntLength cur_dlugosc, max_dlugosc;     /* dĹ�ugoĹ�ci Ĺ�ukĂłw; */
    
    cin >> n;
    
    dlugosci = new IntLength[n];            /* [0..n-1]; */
    
    for (IntCounter i = 0; i < n; i ++) {   /* punktĂłw jest n; */
        cin >> dlugosci[i];                 /* pobieram dĹ�ugoĹ�Ä� Ĺ�uku i; */
        d2_okregu += dlugosci[i];           /* sumujÄ� wszystkie -> dĹ�ugoĹ�Ä�; */
    }
    
    d2_okregu = (d2_okregu + 1) / 2;        /* potrzeba 1/2 dĹ�ugoĹ�ci okrÄ�gu; */
    
    max_dlugosc = 0;
    
    k = 0;
    l = 0;
    
    cur_dlugosc = dlugosci[0];
    
    /* dopĂłki wskaĹşnik nie przebiegnie caĹ�ego okrÄ�gu; */
    while(k < n) {
        /* ucinam pierwszy fragment; */
        cur_dlugosc -= dlugosci[k];
        /* dopĂłki fragment mniejszy niĹź poĹ�owa okrÄ�gu; */
        while(cur_dlugosc < d2_okregu) {
            l++;
            l %= n;
            cur_dlugosc += dlugosci[l];
        }
        /* obecna dĹ�ugoĹ�Ä� jest za duĹźa bo dodano o jeden odcinek za duĹźo; */
        cur_dlugosc -= dlugosci[l --];
        /* jeĹźeli dĹ�ugoĹ�Ä� przekroczyĹ�a poprzedniego maxa - przypisujÄ�; */
        if(max_dlugosc < cur_dlugosc)
            max_dlugosc = cur_dlugosc;
        k ++;
    }
    
    /* zwrĂłcenie wyniku; */
    cout
        << setiosflags (ios_base::fixed)    /* bez formy wykĹ�adniczej; */
        << setprecision(0)                  /* bez cyfr po przecinku; */
        << max_dlugosc
        << endl;
    
    /* zwalnianie pamiÄ�ci; */
    delete[] dlugosci;
    
    return 0;
}
