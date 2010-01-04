#include <iostream>
#include <string>
#include <utility>
#include <deque>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

typedef long integer;
typedef pair<integer, integer> ii;
typedef vector<integer> vi;
typedef deque<pair<ii, ii> > diiii;

void compute_prefix_function(const string& s, integer len, vi& prefix_fun)
{
    prefix_fun[0] = 0;
    integer k = 0;
    for(integer q = 1; q < len; q ++) {
        while(k > 0 && s[k] != s[q])
            k = prefix_fun[k];
        if(s[k] == s[q])
            k ++;
        prefix_fun[q] = k;
    }
}

bool compare_cstr(const char* c_str1, const char* c_str2)
{
    return strcmp(c_str1, c_str2) < 0;
}

void compute_suffix_array(const string& s, integer len, vi& suffix_arr)
{
    const char* str = s.c_str();
    vector<const char*> suffix_vector(len + 1);
    for(integer i = 0; i <= len; i ++)
        suffix_vector[i] = str + i;
    sort(suffix_vector.begin(), suffix_vector.end(), compare_cstr);
    for(integer i = 1; i <= len; i ++)
        suffix_arr[i - 1] = static_cast<integer>(suffix_vector[i] - str);
}

void compute_rank_array(const vi& suffix_arr, integer len, vi& rank_arr)
{
    for(integer i = 0; i < len; i ++)
        rank_arr[suffix_arr[i]] = i;
}

inline integer max(integer a, integer b)
{
    return (a > b) ? a : b;
}

void compute_lcp_array(const vi& suffix_arr, const vi& rank_arr,
        const string& s, integer len, vi& lcp_arr)
{
    integer l = 0;
    for(integer i = 0; i < len; i ++) {
        if(rank_arr[i] > 0) {
            while(s[l + i] == s[l + suffix_arr[rank_arr[i] - 1]])
                l ++;
            lcp_arr[rank_arr[i] - 1] = l;
            l = max(0, l - 1);
        }
    }
}

class interval_set
{
    private:
        vi tree;
        inline integer parent(integer index)
        {
            return index / 2;
        }
        inline integer left(integer index)
        {
            return 2 * index;
        }
        inline integer right(integer index)
        {
            return 2 * index + 1;
        }
    public:
        interval_set(integer size)
            : tree(size + 1)
        {
            tree[0] = size;
        }
        interval_set(const vi& values)
            : tree(size + 1);
        {
            vi::iterator input = tree.rbegin();
            vi::const_iterator output = values.rbegin();
            while(output != values.rend()) {
                *input = *(output ++);
                input ++;
            }

        }
        inline integer size()
        {
            return tree[0];
        }

}

int main()
{
    ios_base::sync_with_stdio(0);

    // Input word length and input queries quantity;
    integer word_length;
    integer queries_quantity;
    cin >> word_length;
    cin >> queries_quantity;

    string input_word;
    cin >> input_word;

    diiii orders;

    for(integer i = 0; i < queries_quantity; i ++) {
        ii first_interval;
        ii second_interval;
        cin >> first_interval.first;
        cin >> first_interval.second;
        cin >> second_interval.first;
        cin >> second_interval.second;
        orders.push_back(make_pair(first_interval, second_interval));
    }

    vi prefix_fun = vi(word_length);
    compute_prefix_function(input_word, word_length, prefix_fun);

    vi suffix_arr = vi(word_length);
    compute_suffix_array(input_word, word_length, suffix_arr);

    vi rank_arr = vi(word_length);
    compute_rank_array(suffix_arr, word_length, rank_arr);

    vi lcp_arr = vi(word_length - 1);
    compute_lcp_array(suffix_arr, rank_arr, input_word, word_length, lcp_arr);

    cout << "STR: ";
    cout << input_word;
    cout << "\nPRE: ";
    for(vi::const_iterator i = prefix_fun.begin();
            i != prefix_fun.end();
            i ++)
        cout << *i << ' ';
    cout << "\nSUF: ";
    for(vi::const_iterator i = suffix_arr.begin();
            i != suffix_arr.end();
            i ++)
        cout << *i << ' ';
    cout << "\nRAN: ";
    for(vi::const_iterator i = rank_arr.begin();
            i != rank_arr.end();
            i ++)
        cout << *i << ' ';
    cout << "\nLCP: ";
    for(vi::const_iterator i = lcp_arr.begin();
            i != lcp_arr.end();
            i ++)
        cout << *i << ' ';
    cout << endl;

    return 0;
}
