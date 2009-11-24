/* Zadanie 1: Punkty na okręgu. */
/* Cezary Bartoszuk, cb277617@students.mimuw.edu.pl */

#include <iostream>                         /* strumienie I/O; */
#include <iomanip>                          /* do wypisania wyniku; */

using namespace std;

typedef long IntCounter;                    /* n: [1..10^6]; */
typedef long double IntLength;              /* suma: [1..10^9]; */

int main() {
    
    ios_base::sync_with_stdio(0);           /* względy wydajnościowe; */
    
    IntCounter k, l;                        /* wskaźniki punktów na okręgu; */
    IntCounter n;                           /* ilość punktów na okręgu; */
    IntLength *dlugosci;                    /* tablica długości; */
    IntLength d2_okregu = 0;                /* pół długości okręgu; */
    IntLength cur_dlugosc, max_dlugosc;     /* długości łuków; */
    
    cin >> n;
    
    dlugosci = new IntLength[n];            /* [0..n-1]; */
    
    for (IntCounter i = 0; i < n; i ++) {   /* punktów jest n; */
        cin >> dlugosci[i];                 /* pobieram długość łuku i; */
        d2_okregu += dlugosci[i];           /* sumuję wszystkie -> długość; */
    }
    
    d2_okregu = (d2_okregu + 1) / 2;        /* potrzeba 1/2 długości okręgu; */
    
    max_dlugosc = 0;
    
    k = 0;
    l = 0;
    
    cur_dlugosc = dlugosci[0];
    
    /* dopóki wskaźnik nie przebiegnie całego okręgu; */
    while(k < n) {
        /* ucinam pierwszy fragment; */
        cur_dlugosc -= dlugosci[k];
        /* dopóki fragment mniejszy niż połowa okręgu; */
        while(cur_dlugosc < d2_okregu) {
            l++;
            l %= n;
            cur_dlugosc += dlugosci[l];
        }
        /* obecna długość jest za duża bo dodano o jeden odcinek za dużo; */
        cur_dlugosc -= dlugosci[l --];
        /* jeżeli długość przekroczyła poprzedniego maxa - przypisuję; */
        if(max_dlugosc < cur_dlugosc)
            max_dlugosc = cur_dlugosc;
        k ++;
    }
    
    /* zwrócenie wyniku; */
    cout
        << setiosflags (ios_base::fixed)    /* bez formy wykładniczej; */
        << setprecision(0)                  /* bez cyfr po przecinku; */
        << max_dlugosc
        << endl;
    
    /* zwalnianie pamięci; */
    delete[] dlugosci;
    
    return 0;
}
