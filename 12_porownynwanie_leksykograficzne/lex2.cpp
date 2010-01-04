#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
#include <set>
#include <utility>

using namespace std;

typedef long integer;
typedef double real;

inline integer largest_pow_two(integer n)
{
    real lg_n_floor = floor(log(static_cast<real>(n)) / log(2.0));
    return static_cast<integer>(lg_n_floor);
}

inline integer pow_two(integer exp)
{
    return 1 << exp;
}

typedef vector<integer> vi;
typedef vector<vi> vvi;
typedef pair<integer, integer> ii;
typedef pair<ii, integer> iii;

int main()
{
    ios::sync_with_stdio(0);

    integer input_len, orders_q;

    cin >> input_len;
    cin >> orders_q;

    string input_str;

    cin >> input_str;

    integer max_exp = largest_pow_two(input_len);

    vvi data(max_exp + 1);
    for(integer i = 0; i <= max_exp; i ++) {
        data[i] = vi(input_len - pow_two(i) + 1);
    }

    for(integer i_st = 0; i_st < input_len; i_st ++)
        data[0][i_st] = (static_cast<integer>(input_str.at(i_st)));
    for(integer sub_exp = 1; sub_exp <= max_exp; sub_exp ++) {
        integer sub_len = pow_two(sub_exp);
        set<iii> temp;
        //for(integer i_st = 0; i_st + sub_len - 1 < input_len; i_st ++) {
            //temp.insert(make_pair(
                        //make_pair(data[sub_exp - 1][i_st],
                            //data[sub_exp - 1][i_st + sub_len / 2]),
                        //i_st));
        //}
        ii prev = make_pair(-1, -1);
        integer index = 0;
        for(set<iii>::const_iterator iter = temp.begin();
                iter != temp.end();
                iter ++)
        {
            if(iter->first != prev) {
                index ++;
                prev = iter->first;
            }
            data[sub_exp][iter->second] = index;
        }
    }

    for(integer i = 0; i < orders_q; i ++) {
        integer first_beg, first_end, second_beg, second_end;
        cin >> first_beg;
        cin >> first_end;
        cin >> second_beg;
        cin >> second_end;
        integer first_len = first_end - first_beg;
        integer second_len = second_end - second_beg;
        integer minimum = first_len < second_len ? first_len : second_len;
        integer max_pow_exp = largest_pow_two(minimum);
        integer ff = first_beg;
        integer fs = first_beg + minimum - pow_two(max_pow_exp);
        integer sf = second_beg;
        integer ss = second_beg + minimum - pow_two(max_pow_exp);
        if(data[max_pow_exp][ff] < data[max_pow_exp][sf])
            cout << "<\n";
        else if(data[max_pow_exp][ff] > data[max_pow_exp][sf])
            cout << ">\n";
        else {
            if(data[max_pow_exp][fs] < data[max_pow_exp][ss])
                cout << "<\n";
            else if(data[max_pow_exp][fs] > data[max_pow_exp][ss])
                cout << ">\n";
            else {
                if(first_len < second_len)
                    cout << "<\n";
                else if(first_len > second_len)
                    cout << ">\n";
                else
                    cout << "=\n";
            }
        }
    }

    cout << flush;

    return 0;
}
