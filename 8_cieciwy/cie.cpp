#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <functional>
#include <cassert>

using namespace std;

typedef long long integer;

typedef pair<integer, integer> ii;
typedef pair<ii, integer> iii;
typedef vector<iii> viii;

class itree
{
    private:
        typedef vector<integer> vi;
        vi nodes; // nodes[0] unused;
        integer n;
        static inline integer parent(integer i) { return i / 2; }
        static inline integer left(integer i) { return 2 * i; }
        static inline integer right(integer i) { return 2 * i + 1; }
        static inline integer min(integer i, integer j)
        {
            return (i < j) ? i : j;
        }
        static inline integer max(integer i, integer j)
        {
            return (i > j) ? i : j;
        }
        static inline integer avg(integer i, integer j)
        {
            return (i + j) / 2;
        }
        static inline integer abs(integer i) { return max(i, -i); }
    public:
        itree(integer n)
            : nodes(2 * n + 1, 0),
              n(n)
        {
        }
        void push_prefix(integer prefix_end_inclusive, integer val)
        {
            integer ibegin = 1, iend = n;
            integer index = 1;
            while(true) {
                //tu jest źle....
                integer med = avg(ibegin, iend);
                if(prefix_end_inclusive <= med) {
                    if(ibegin == iend) {
                        nodes[index] += val;
                        break;
                    }
                    else {
                        iend = med;
                        index = left(index);
                    }
                }
                else { // prefix_end_inclusive > med
                    if(prefix_end_inclusive >= iend) { // ==
                        nodes[index] += val;
                        break;
                    }
                    else {
                        nodes[left(index)] += val;
                        ibegin = med + 1;
                        index = right(index);
                    }
                }
            }
        }
        integer count(integer i) const
        {
            integer ibegin = 1, iend = n;
            integer index = 1;
            integer counter = 0;
            while(index <= 2 * n) {
                //assert(index <= 2 * n);
                counter += nodes[index];
                integer med = avg(ibegin, iend);
                if(i <= med) {
                    iend = med;
                    index = left(index);
                }
                else {
                    ibegin = med + 1;
                    index = right(index);
                }
            }
            return counter;
        }
        void clear() {
            nodes.assign(2 * n + 1, 0);
        }
        friend ostream& operator<<(ostream& os, const itree& i);
};

ostream& operator<<(ostream& os, const itree& i)
{
    for(integer j = 1; j <= i.n; j ++)
        os << i.count(j) << '\n';
    return os;
}

bool colour_cmp(iii one, iii two)
{
    return one.second < two.second;
}

int main()
{
    ios_base::sync_with_stdio(0);

    integer n, k;
    cin >> n;
    cin >> k;

    viii circle;
    circle.reserve(n);

    for(integer i = 0; i < n; i ++) {
        integer a, b, c;
        cin >> a;
        cin >> b;
        cin >> c;
        if(a < b)
            circle.push_back(make_pair(make_pair(a, b), c));
        else
            circle.push_back(make_pair(make_pair(b, a), c));
    }

    integer counter = 0;

    sort(circle.begin(), circle.end(), less<iii>()); // sorting with first coord

    itree all(2 * n);
    for(viii::const_iterator iter = circle.begin();
            iter != circle.end(); iter ++) {
        counter += all.count(iter->first.first) - all.count(iter->first.second);
        //all.push_prefix(iter->first.second, 1);
    }

    stable_sort(circle.begin(), circle.end(), colour_cmp); // sorting with col

    all.clear();
    integer cur_colour = circle.begin()->second;
    for(viii::const_iterator iter = circle.begin();
            iter != circle.end(); iter ++) {
        if(iter->second != cur_colour) {
            all.clear();
            cur_colour = iter->second;
        }
        counter -= all.count(iter->first.first) - all.count(iter->first.second);
        all.push_prefix(iter->first.second, 1);
    }

    cout << counter << flush;

    return 0;
}
