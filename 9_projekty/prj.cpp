#include<iostream>
#include<set>
#include<map>
#include<algorithm>

typedef unsigned long UInteger;
typedef unsigned long ULongInt;

const UInteger MAX_N = 100001;
const UInteger MAX_M = 500001;
const ULongInt MAX_PROG = 100000000;

ULongInt prj_programmers[MAX_N];
UInteger q_pointing_at_me[MAX_N];
std::set<UInteger> free_verticles = std::set<UInteger>();
//std::map<UInteger, std::set<UInteger> > edges =
//    std::map<UInteger, std::set<UInteger> >();
std::set<UInteger> edges[MAX_M];

bool use_heap = false;

bool compare(UInteger i, UInteger j)
{
    return prj_programmers[i] < prj_programmers[j];
}

bool(*cmp)(UInteger, UInteger) = compare;
typedef std::set<UInteger, bool(*)(UInteger,UInteger)> Heap;
Heap heap(cmp);

void addEdge(UInteger from, UInteger to)
{
    edges[from].insert(to);
    q_pointing_at_me[to] ++;
    free_verticles.erase(to);
}

void addVerticle(UInteger vert)
{
    edges[vert] = std::set<UInteger>();
    free_verticles.insert(vert);
    q_pointing_at_me[vert] = 0;
}

static void decrease_pointing_at_me(UInteger vert)
{
    if((-- q_pointing_at_me[vert]) == 0) {
        if(use_heap == false)
            free_verticles.insert(vert);
        else
            heap.insert(vert);
    }
}

void removeVerticle(UInteger vert)
{
    void(*dec)(UInteger) = decrease_pointing_at_me;
    std::set<UInteger> & v = edges[vert];
    //std::for_each(v.begin(), v.end(), dec);
    for(std::set<UInteger>::const_iterator iter = v.begin();
            iter != v.end();
            iter ++)
        dec(*iter);
    //edges.erase(vert);
    edges[vert] = std::set<UInteger>();
}

inline UInteger max(UInteger a, UInteger b)
{
    return (a >= b) ? a : b;
}

int main()
{
    std::ios_base::sync_with_stdio(0);
    UInteger n, m, k;

    std::cin >> n;  // liczba projektow
    std::cin >> m;  // liczba zaleznosci miedzy projektami
    std::cin >> k;  // min projektow do zrealizowania

    use_heap = false;

    for(UInteger i = 1; i <= n; i ++) {
        std::cin >> prj_programmers[i];
        addVerticle(i);
    }
    for(UInteger i = 1; i <= m; i ++) {
        UInteger a, b; // a <- b "a wymaga b"
        std::cin >> a;
        std::cin >> b;
        addEdge(b, a);
    }
    for(std::set<UInteger>::iterator i = free_verticles.begin();
            i != free_verticles.end();
            i ++) {
        heap.insert(*i);
        //std::cerr << ":" << *i << std::endl;
    }
    use_heap = true;
    UInteger max_cost = 0;
    for(UInteger i = 1; i <= k; i ++) {
        UInteger v = *(heap.begin());
        heap.erase(heap.begin());
        //std::cerr << prj_programmers[v] << std::endl;
        max_cost = max(max_cost, prj_programmers[v]);
        removeVerticle(v);
    }

    std::cout << max_cost << std::endl;

    return 0;
}
