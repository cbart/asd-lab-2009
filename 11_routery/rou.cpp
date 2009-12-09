#include <iostream>
#include <vector>

using namespace std;

typedef long integer;
typedef pait<integer, integer> ii;
typedef vector<ii> vii;

int main()
{
    ios_base::sync_with_stdio(0);

    integer vertices_q, edges_q;

    vii input;
    input.reserve(edges_q);
    for(integer i = 0; i < edges_q; i ++) {
        integer a, b;
        cin >> a;
        cin >> b;
        if(a < b)
            input.push_back(make_pair(a, b));
        else
            input.push_back(make_pair(b, a));
    }

    //TODO: write...

    return 0;
}
