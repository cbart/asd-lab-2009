#include <iostream>
#include <limits>

typedef long long Integer;
typedef long long BigInteger;
typedef bool Boolean;

const Integer MAX_LENGTH = 100001;
const BigInteger MAX_SIZE = 1000000;

BigInteger input[MAX_LENGTH];

inline BigInteger value(Integer i) { return input[i]; }

struct MaxHeap
{
    Integer size;
    Integer array[MAX_LENGTH]; /* heap -> ids */
    Integer rev_array[MAX_LENGTH]; /* ids -> heap */

    MaxHeap(): size(0) {};
    inline Integer parent(Integer i) { return i / 2; }
    inline Integer left(Integer i) { return 2 * i; }
    inline Integer right(Integer i) { return (2 * i) + 1; }
    void swap(Integer i, Integer j);
    void maxHeapify(Integer i);

    inline Integer max_id() { return array[1]; }
    void extract_id(Integer id);
    void insert(Integer id);
    inline Boolean empty() { return (size == 0); };

};

void MaxHeap::swap(Integer i, Integer j) /* i, j - indeksy w heap; */
{
    Integer tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
    tmp = rev_array[array[i]];
    rev_array[array[i]] = rev_array[array[j]];
    rev_array[array[j]] = tmp;
}

void MaxHeap::maxHeapify(Integer i) /* i - indeks w heap; */
{
    Integer largest;
    Integer l = left(i);
    Integer r = right(i);
    if((l <= size) && (value(array[l]) > value(array[i])))
        largest = l;
    else
        largest = i;
    if((r <= size) && (value(array[r]) > value(array[largest])))
        largest = r;
    if(largest != i) {
        swap(i, largest);
        maxHeapify(largest);
    }
}

void MaxHeap::extract_id(Integer id) /* id - indeks w input */
{
    Integer i = rev_array[id];
    swap(i, size);
    size --;
    while((i > 1) && (value(array[parent(i)]) < value(array[i]))) {
        swap(i, parent(i));
        i = parent(i);
    }
    maxHeapify(i);
}

void MaxHeap::insert(Integer id) /* id - indeks w input */
{
    size ++;
    array[size] = id;
    rev_array[id] = size;
    Integer i = size;
    while((i > 1) && (value(array[parent(i)]) < value(array[i]))) {
        swap(i, parent(i));
        i = parent(i);
    }
}

struct MinHeap
{
    Integer size;
    Integer array[MAX_LENGTH]; /* heap -> ids */
    Integer rev_array[MAX_LENGTH]; /* ids -> heap */

    MinHeap(): size(0) {};
    inline Integer parent(Integer i) { return i / 2; }
    inline Integer left(Integer i) { return 2 * i; }
    inline Integer right(Integer i) { return (2 * i) + 1; }
    void swap(Integer i, Integer j);
    void minHeapify(Integer i);

    inline Integer min_id() { return array[1]; }
    void extract_id(Integer id);
    void insert(Integer id);
    inline Boolean empty() { return (size == 0); };

};

void MinHeap::swap(Integer i, Integer j) /* i, j - indeksy w heap; */
{
    Integer tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
    tmp = rev_array[array[i]];
    rev_array[array[i]] = rev_array[array[j]];
    rev_array[array[j]] = tmp;
}

void MinHeap::minHeapify(Integer i) /* i - indeks w heap; */
{
    Integer smallest;
    Integer l = left(i);
    Integer r = right(i);
    if((l <= size) && (value(array[l]) < value(array[i])))
        smallest = l;
    else
        smallest = i;
    if((r <= size) && (value(array[r]) < value(array[smallest])))
        smallest = r;
    if(smallest != i) {
        swap(i, smallest);
        minHeapify(smallest);
    }
}

void MinHeap::extract_id(Integer id) /* id - indeks w input */
{
    Integer i = rev_array[id];
    swap(i, size);
    size --;
    while((i > 1) && (value(array[parent(i)]) > value(array[i]))) {
        swap(i, parent(i));
        i = parent(i);
    }
    minHeapify(i);
}

void MinHeap::insert(Integer id) /* id - indeks w input */
{
    size ++;
    array[size] = id;
    rev_array[id] = size;
    Integer i = size;
    while((i > 1) && (value(array[parent(i)]) > value(array[i]))) {
        swap(i, parent(i));
        i = parent(i);
    }
}


Integer median_id = -1;
MinHeap upper = MinHeap();
MaxHeap lower = MaxHeap();
BigInteger upper_sum = 0;
BigInteger lower_sum = 0;

void shifter_balance()
{
    if(upper.size > lower.size) { /* upper -> lower; */
        Integer upper_min_id = upper.min_id();
        upper.extract_id(upper_min_id);
        upper_sum -= value(upper_min_id);
        lower_sum += value(upper_min_id);
        lower.insert(upper_min_id);
    }
    else if(upper.size < lower.size - 1) { /* lower -> upper */
        Integer lower_max_id = lower.max_id();
        lower.extract_id(lower_max_id);
        lower_sum -= value(lower_max_id);
        upper_sum += value(lower_max_id);
        upper.insert(lower_max_id);
    }
    median_id = lower.max_id();
//std::cerr << "balance: median=" << median_id << std::endl;
}

void shifter_insert(Integer id)
{
    if(median_id == -1) {
        median_id = id;
        lower_sum += value(id);
        lower.insert(median_id);
    }
    else {
        if(value(id) <= value(median_id)) {
            lower.insert(id);
            lower_sum += value(id);
        }
        else {
            upper.insert(id);
            upper_sum += value(id);
        }
    }
    shifter_balance();
}

void shifter_erase(Integer id)
{
    if(value(id) <= value(median_id)) {
        lower.extract_id(id);
        lower_sum -= value(id);
    }
    else {
        upper.extract_id(id);
        upper_sum -= value(id);
    }
    shifter_balance();
}

inline BigInteger abs(BigInteger a)
{
    return (a < 0) ? (-a) : a;
}

int main()
{
    std::ios_base::sync_with_stdio(0);

    /* wczytaj n, k; */
    Integer n, k;
    std::cin >> n;
    std::cin >> k;

    for(Integer i = 0; i < n; i ++)
        std::cin >> input[i];

    median_id = -1;
    upper_sum = 0;
    lower_sum = 0;

    for(Integer i = 0; i < k - 1; i ++)
        shifter_insert(i);

    Integer opt_median_id;
    BigInteger opt_cost = std::numeric_limits<BigInteger>::max();
    Integer cur_median_id;
    BigInteger cur_cost;
    Integer opt_start;
    for(Integer i = k - 1; i < n; i ++) {
        shifter_insert(i);
//std::cerr << "Krok " << i - k << ".\n" << std::flush;
        cur_median_id = median_id;
        BigInteger upper_cost = upper_sum - ((BigInteger) upper.size *
                value(median_id));
        BigInteger lower_cost = ((BigInteger) lower.size * value(median_id)) -
                lower_sum;
        cur_cost = upper_cost + lower_cost;
//std::cerr << "     mediana: [" << cur_median_id << "]=" << value(cur_median_id)
//    << ", koszt: " << cur_cost << ".\n" << std::flush;
        if(cur_cost < opt_cost) {
            opt_cost = cur_cost;
            opt_median_id = cur_median_id;
            opt_start = i - k + 1;
        }
        shifter_erase(i - k + 1);
    }

    Integer opt_median = value(opt_median_id);
    Integer mycost = 0;
    for(Integer i = 0; i < k; i ++) {
        mycost += abs(input[i + opt_start] - opt_median);
        input[i + opt_start] = opt_median;
    }

    std::cout << mycost << '\n';
    for(Integer i = 0; i < n; i ++)
        std::cout << input[i] << '\n';
    std::cout << std::flush;
    //MinHeap mheap = MinHeap();
    //for(Integer i = 0; i < n; i ++)
        //mheap.insert(i);
    //for(Integer i = 0; i < n; i += 2)
        //mheap.extract_id(i);
//
    //while(!mheap.empty()) {
        //std::cerr << input[mheap.min_id()] << " ";
        //mheap.extract_id(mheap.min_id());
    //}
    //std::cerr << std::endl;


    return 0;
}
