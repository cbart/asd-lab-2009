#include <iostream>
#include <algorithm>
#include <utility> //pair
#include <vector>
#include <limits>
#include <queue>
#include <deque>
#include <functional>

using namespace std;

typedef long integer;

typedef pair<integer, integer> ii; //edge = pair (cost, to_vertex)
typedef deque<ii> vii; // vertex = deque of edges
typedef vector<vii> vvii; //graph = vector of vertexes
typedef vvii graph;
typedef vector<integer> vi;

const integer infty = numeric_limits<integer>::max();

vi dijkstra(integer from, const graph& g, integer n)
{
    vi dijkstra_len(n, infty);
    priority_queue<ii, vector<ii>, greater<ii> > pri_queue;

    dijkstra_len[from] = 0;
    pri_queue.push(ii(0, from));

    while(!pri_queue.empty()) {
        ii top = pri_queue.top();
        pri_queue.pop();
        integer v = top.second, d = top.first;
        if(d <= dijkstra_len[v]) {
            for(vii::const_iterator it = g[v].begin();
                    it != g[v].end(); it ++) {
                integer v2 = it->first, cost = it->second;
                if(dijkstra_len[v2] > dijkstra_len[v] + cost) {
                    dijkstra_len[v2] = dijkstra_len[v] + cost;
                    pri_queue.push(ii(dijkstra_len[v2], v2));
                }
            }
        }
    }
    return dijkstra_len;
}

inline integer min(integer i1, integer i2) { return (i1 < i2) ? i1 : i2; }
inline integer max(integer i1, integer i2) { return (i1 > i2) ? i1 : i2; }

int main()
{
    ios_base::sync_with_stdio(0);

    integer n, m;
    cin >> n;
    cin >> m;

    graph g(n); // graph
    graph rev(n); // reversed graph `g`

    for(integer i = 0; i < m; i ++) {
        integer a, b, c;
        cin >> a;
        cin >> b;
        cin >> c;
        g[a].push_back(ii(b, c));
        rev[b].push_back(ii(a, c));
    }

    vi dijkstra_from_beginning = dijkstra(0, g, n);
    vi dijkstra_from_end = dijkstra(n - 1, rev, n);

    integer result = infty;

    for(integer i = 1; i < n; i ++)
        result = min(result,
                max(dijkstra_from_beginning[i], dijkstra_from_end[i]));

    cout << result << flush;

    return 0;
}
