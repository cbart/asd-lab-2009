#include <iostream>
#include <algorithm>

typedef long long Integer;
typedef bool Boolean;

//const Integer CIRCLE_ARRAY_SIZE = 200001;

struct Chord
{
    public:
        Integer begin;
        Integer end;
        Integer colour;
};

typedef Integer Index;
typedef Integer Value;

class IntervalSet
{
    private:
        class Node
        {
            public:
                const Index begin;
                const Index end;
            private:
                Value quantity;
                Node *left;
                Node *right;
            public:
                Node(Index l, Index r);
                Value count(Index i, Value akk) const;
                void addPrefix(Index to, Value q);
                void addSuffix(Index from, Value q);
                ~Node();
        };
        Node *root;
    public:
        IntervalSet(Index from, Index to) : root(new Node(from, to)) {}
        inline void addPrefix(Index to, Value q) { root->addPrefix(to, q); }
        inline void addSuffix(Index from, Value q) { root->addSuffix(from, q); }
        inline Value count(Index i) { return root->count(i, 0); }
        virtual ~IntervalSet() { delete root; }
        void print()
        {
            for(Integer i = root->begin; i <= root->end; i ++)
                std::cerr << "#[" << i << "]=" << count(i) << "; ";
            std::cerr << std::flush;
        }
};

IntervalSet::Node::Node(Integer l, Integer r)
    : begin(l),
      end(r),
      quantity(0)
{
    if(l < r) {
        Integer mid = (l + r) / 2;
        left = new Node(l, mid);
        right = new Node(mid + 1, r);
    }
    else {
        left = NULL;
        right = NULL;
    }
}

IntervalSet::Node::~Node()
{
    delete left;
    delete right;
}

Value IntervalSet::Node::count(Index i, Value akk = 0) const
{
    if(begin > i || i > end)
        return 0;
    if(begin == end)
        return quantity + akk;
    if(i <= ((begin + end) / 2))
        return left->count(i, quantity + akk);
    else
        return right->count(i, quantity + akk);
}

void IntervalSet::Node::addPrefix(Index to, Value q)
{
    if(end == to)
        quantity += q;
    else if(to >= begin) {
        Index mid = (begin + end) / 2;
        if(to <= mid)
            left->addPrefix(to, q);
        else {
            left->addPrefix(mid, q);
            right->addPrefix(to, q);
        }
    }
}

void IntervalSet::Node::addSuffix(Index from, Value q)
{
    if(begin == from)
        quantity += q;
    else if(from <= end) {
        Index mid = (begin + end) / 2;
        if(from <= mid) {
            left->addSuffix(from, q);
            right->addSuffix(mid + 1, q);
        }
        else
            right->addSuffix(from, q);
    }
}

class ChordSet
{
    private:

        Integer crosses;
        IntervalSet prefixes;
        IntervalSet suffixes;

    public:

        ChordSet(Integer n)
            : crosses(0),
              prefixes(1, 2 * n),
              suffixes(1, 2 * n)
        {
        }

        void add(Integer a, Integer b)
        {
            Integer pref_crosses = prefixes.count(a) + prefixes.count(b);
            Integer suff_crosses = suffixes.count(a) + suffixes.count(b);
            //std::cerr << "Adding crosses (" << a << "; " << b << "): "
                //<< left_crosses << " - " << right_crosses << "." << std::endl;
            crosses += (pref_crosses > suff_crosses) ?
                (pref_crosses - suff_crosses) :
                (suff_crosses - pref_crosses);
            prefixes.addPrefix(b, 1);
            suffixes.addSuffix(a, 1);
        }

        inline Integer crossesQuantity() const
        {
            return crosses;
        }

        void print()
        {
            std::cerr << "Prefixes: ";
            prefixes.print();
            std::cerr << "\nSuffixes: ";
            suffixes.print();
            std::cerr << std::endl;
        }
};

bool chordColourCompare(Chord a, Chord b)
{
    return (a.colour <= b.colour);
}

int main()
{
    // Do not sync with stdio.
    std::ios_base::sync_with_stdio(0);

    Integer n, k;
    std::cin >> n;
    std::cin >> k;

    Chord* circle = new Chord[n];
    for(Integer i = 0; i < n; i ++) {
        Integer a, b;
        std::cin >> a;
        std::cin >> b;
        std::cin >> circle[i].colour;
        if(a < b) {
            circle[i].begin = a;
            circle[i].end = b;
        }
        else {
            circle[i].begin = b;
            circle[i].end = a;
        }
    }

    std::sort(circle, circle + n, chordColourCompare);

    //for(Integer i = 0; i < n; i ++)
        //std::cerr << circle[i].colour << "."
            //<< "[" << circle[i].begin << "; " << circle[i].end << "]; "
            //<< std::flush;
//
    ChordSet all = ChordSet(n);
    Integer same_colour_crosses = 0;
    Integer i = 0;
    while(i < n) {
        ChordSet cur_set = ChordSet(n);
        Integer cur_colour = circle[i].colour;
        while(i < n && cur_colour == circle[i].colour) {
            all.add(circle[i].begin, circle[i].end);
            cur_set.add(circle[i].begin, circle[i].end);
            i ++;
        }
        same_colour_crosses += cur_set.crossesQuantity();
    }
    std::cout << all.crossesQuantity() - same_colour_crosses << std::endl;

    return 0;
}

