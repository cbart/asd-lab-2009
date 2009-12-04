// ASD-LAB 2009: TASK 8.
// AUTHOR: Cezary Bartoszuk <cbart@students.mimuw.edu.pl>
// ID: cb277617

#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <utility>
#include <functional>

using namespace std;

// Integer types.
typedef long integer;
typedef long long big_integer;

// Non-integer types.
typedef pair<integer, integer> ii; //  Pair of integers - a chord.
typedef pair<ii, integer> iii;  // Triple of integers - a chord with colour.
typedef vector<iii> viii;  // Sequence of triples of integers.

// Computes maximum of given integers.
inline integer max(integer i, integer j)
{
    return (i > j) ? i : j;
}

// Computes average of given integers.
inline integer avg(integer i, integer j)
{
    return (i + j) / 2;
}

// Computes absolute value of given integer.
inline integer abs(integer i)
{
    return max(i, -i);
}

// Multi-interval tree.
class itree
{

    private:

        // Sequence of integers.
        typedef vector<integer> vi;

        // Tree is stored in an array like in heap implementation.
        // `nodes[0]` is quantum of max interval, eg. [1..nodes[0]]
        // is max we can store here.
        vi nodes;

        // Sequence of modified elements.
        deque<integer> modified;

        // Computes index of parent of element with given index `i`.
        static inline integer parent(integer i) { return i / 2; }

        // Computes index of left son of element with given index `i`.
        static inline integer left(integer i) { return 2 * i; }

        // Computes index of right son of element with given index `i`.
        static inline integer right(integer i) { return 2 * i + 1; }

    public:

        // Creates itree with maximal interval stored [1..quantity].
        //   Time: O(quantity).
        itree(integer quantity)
            : nodes(2 * quantity + 1, 0),
              modified()
        {
            nodes[0] = quantity;
        }

        // Inserts `val` of prefixex with given end to `self`.
        //   Time: O(log n) where `n` is size of itree.
        void push_prefix(integer prefix_end_inclusive, integer val)
        {
            integer ibegin = 1, iend = nodes[0];
            integer index = 1;
            while(index <= 2 * nodes[0]) {
                integer med = avg(ibegin, iend);
                if(prefix_end_inclusive <= med) {
                    if(ibegin == iend) {
                        nodes[index] += val;
                        modified.push_back(index);
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
                        modified.push_back(index);
                        break;
                    }
                    else {
                        nodes[left(index)] += val;
                        modified.push_back(left(index));
                        ibegin = med + 1;
                        index = right(index);
                    }
                }
            }
        }

        // Computes how many sets which contains `i` are in `self`.
        //   Time: pesimistic O(lg n) where `n` is size of itree.
        integer count(integer i) const
        {
            integer ibegin = 1, iend = nodes[0];
            integer index = 1;
            integer counter = 0;
            while(index <= 2 * nodes[0]) {
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

        // Clears the set.
        //   Time: pesimistic O(m log n) where `m` is # of inserts
        //       and `n` is size of itree.
        void clear() {
            while(!modified.empty()) {
                nodes[modified.front()] = 0;
                modified.pop_front();
            }

        }

};

// Compares colours of given chords.
bool colour_cmp(iii one, iii two)
{
    return one.second < two.second;
}

// Indicates if `element` is in range described by `from` and `to`.
// The range is left-inclusive and right-exclusive (as always).
inline bool in_range(integer element, integer from, integer to)
{
    return (from <= element) && (element < to);
}

int main()
{
    ios_base::sync_with_stdio(0);  // For speed reasons ;).

    // Get all data from stdin.
    integer n, k;  // Chords and colours quantity.
    cin >> n;
    cin >> k;

    viii circle;
    circle.reserve(n);  // Here we store the chords.

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

    // Here we'll be counting the chord crosses.
    big_integer counter = 0;

    // Sorting with first coordinate, ascending.
    sort(circle.begin(), circle.end(), less<iii>());

    // Here we'll compute crossing all chords.
    itree chords(2 * n);
    for(viii::const_iterator iter = circle.begin();
            iter != circle.end();
            iter ++) {
        // Add crosses.
        counter += chords.count(iter->first.first)
            - chords.count(iter->first.second);
        chords.push_prefix(iter->first.second, 1);
    }

    // Sorting with colours, stable, ascending.
    stable_sort(circle.begin(), circle.end(), colour_cmp);

    // Clear all prefixes from `chords`.
    chords.clear();
    // Set the first colour.
    integer cur_colour = circle.begin()->second;
    // Here we'll compute crossing chords with same colour.
    for(viii::const_iterator iter = circle.begin();
            iter != circle.end(); iter ++) {
        if(cur_colour != iter->second) {  // When colour changes:
            chords.clear();
            cur_colour = iter->second;
        }
        // Subtract crosses.
        counter -= chords.count(iter->first.first)
            - chords.count(iter->first.second);
        chords.push_prefix(iter->first.second, 1);
    }

    // Print the result.
    cout << counter << flush;

    return 0;

}  ///:~
