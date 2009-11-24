#include <iostream>
#include <stack>

typedef long Integer;

typedef long long LongInteger;

const Integer DEBUG_LV = 0;

class TimeMultiset {

    protected:

        struct Node;

        Node** time_line;
        Integer next_time;
        Integer max_time;
        std::stack<Node*> all_nodes;

        struct Node {
            Integer min;
            Integer max;
            Integer quantum;
            Integer part;
            Node* left;
            Node* right;

            void printTree(Integer depth)
            {
                for(Integer i = 0; i < depth; i ++)
                    std::cerr << "  ";
                std::cerr << "[" << min << "; " << max
                    << "]@" << part << "; Q=" << quantum << ";\n";
                std::cerr << std::flush;
                if(left != NULL)
                    left->printTree(depth + 1);
                if(right != NULL)
                    right->printTree(depth + 1);
            }


            Node(Integer from, Integer to, std::stack<Node*>& all_nodes)
              : min(from),
                max(to),
                quantum(0)
            {
                if(DEBUG_LV > 0)
                    std::cerr << "Constructing tree [" << from << "; "
                        << to << "];\n" << std::flush;
                if(from < to) {
                    part = (from + to) / 2;
                    left = new Node(from, part, all_nodes);
                    right = new Node(part + 1, to, all_nodes);
                }
                else {
                    part = -1;
                    left = static_cast<Node *>(NULL);
                    right = static_cast<Node *>(NULL);
                }
                /* for destruction purposes... */
                all_nodes.push(this);
            }

            Node(Integer from, Integer to, Integer quantum,
                    Integer part, Node* left, Node* right,
                    std::stack<Node*>& all_nodes)
              : min(from),
                max(to),
                quantum(quantum),
                part(part),
                left(left),
                right(right)
            {
                all_nodes.push(this);
            }

            Node* modifyTo
                (Integer to,
                 Integer delta,
                 std::stack<Node*>& all_nodes)
            {
                if(delta == 0)
                    return this;
                if(part == -1) {
                    if(max <= to)
                        return new Node
                            (min, max, quantum + delta, part, left, right,
                             all_nodes);
                    else
                        return this;
                }
                else {
                    if(max == to)
                        return new Node
                            (min, max, quantum + delta, part, left, right,
                             all_nodes);
                    else /* max > to */
                        if(part < to)
                            return new Node
                                (min,
                                 max,
                                 quantum,
                                 part,
                                 left->modifyTo(part, delta, all_nodes),
                                 right->modifyTo(to, delta, all_nodes),
                                 all_nodes);
                        else
                            return new Node
                                (min,
                                 max,
                                 quantum,
                                 part,
                                 left->modifyTo(to, delta, all_nodes),
                                 right,
                                 all_nodes);
                }
            }

            Integer value(Integer index)
            {
                if(index > max || index < min)
                    return 0;
                else {
                    if(part == -1)  /* index == min == max; */
                        return quantum;
                    else {
                        if(index <= part)
                            return quantum + left->value(index);
                        else
                            return quantum + right->value(index);
                    }
                }
            }

        };

    public:

        void printMultiSet(Integer time)
        {
            std::cerr << "{";
            for(Integer i = 1; i <= max_time; i ++)
                std::cerr << i << ":" << time_line[time]->value(i) << "; ";
            std::cerr << "\b\b}\n" << std::flush;
        }

        void printTree(Integer time)
        {
            time_line[time]->printTree(0);
        }

        TimeMultiset(Integer timelen)
          : time_line(new Node*[timelen + 1]),
            next_time(1),
            max_time(timelen),
            all_nodes(std::stack<Node*>())
        {
            if(DEBUG_LV > 0)
                std::cerr << "Creating TimeMultiset with max_time=" << timelen
                    << " and interval=[1; " << timelen << "];\n" << std::flush;
            time_line[0] = new Node(1, timelen, all_nodes);
        }

        void modify(Integer from, Integer to, Integer delta)
        {
            if(DEBUG_LV > 0)
                std::cerr << "modify(" << from << ", "
                    << to << ", " << delta << ");\n"
                    << std::flush;
            Node* time_tmp = time_line[next_time - 1]->modifyTo(to, delta,
                    all_nodes);
            if(from > 1)
                time_line[next_time] = time_tmp->modifyTo(from - 1, -delta,
                        all_nodes);
            else
                time_line[next_time] = time_tmp;
            if(DEBUG_LV > 0) {
                std::cerr << "Made changes (t=" << next_time << "):\n";
                time_line[next_time]->printTree(0);
                std::cerr << std::flush;
            }
            next_time ++;
        }

        LongInteger getValue(Integer index, Integer time)
        {
            if(DEBUG_LV > 0)
                std::cerr << "getValue(" << index << ", " << time << ");\n" <<
                    std::flush;
            return time_line[time]->value(index);
        }

        ~TimeMultiset()
        {
            if(DEBUG_LV > 0)
                std::cerr << "~TimeMultiset();\n" << std::flush;
            while(!all_nodes.empty())
            {
                Node* to_del = all_nodes.top();
                all_nodes.pop();
                delete to_del;
            }
        }

};

int main()
{
    std::ios_base::sync_with_stdio(0);

    Integer n; /* Quantum of elements in sequence. */

    std::cin >> n;

    TimeMultiset m_set = TimeMultiset(n);

    LongInteger i, j, c, io, t;
    LongInteger x = 0;

    for(Integer T = 0; T < n; T ++) {
        LongInteger x_T_qb = (x + T) * (x + T) * (x + T);
        i = 1 + (x_T_qb % n);
        j = i + (x_T_qb % (n - i + 1));
        c = x_T_qb % 10;
        io = 1 + (((x + 1 + T) * (x + 1 + T) * (x + 1 + T)) % n);
        t = (x + 1234567) % (T + 1);
        if(DEBUG_LV > 0)
            std::cerr << "\nZWIEKSZ " << i << " " << j
                << " " << c << " PYTANIE " << io << " " << t
                << "\n" << std::flush;
        m_set.modify(i, j, c);
        x = m_set.getValue(io, t);
        if(DEBUG_LV > 0) {
            std::cerr << "wynik to " << x << std::endl;
            std::cerr << "multiset(t=" << T + 1 << "): ";
            m_set.printMultiSet(T + 1);
            std::cerr << std::endl;
        }
    }

    for(Integer k = 1; k <= n; k ++)
        std::cout << m_set.getValue(k, n) << '\n';
    std::cout << std::flush;

    return 0;
}
