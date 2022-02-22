#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

ifstream fin("date.txt");

class MinMaxHeap
{
    int dim;
    vector <int> heap;

    //vedem pe ce nivel se afla
    int level(int i)
    {
        int p = 1, ct = 2;
        while (ct <= i)
        {
            ct *= 2;
            p++;
        }
        return p - 1;
    }

    //nivel par -> minheap, altfel maxheap
    void pushDown(int i)
    {
        if (level(i) % 2 == 0) pushDownMin(i);
        else pushDownMax(i);
    }
    void pushDownMin(int m)
    {
        while (m * 2 <= dim)
        {

            //cel mai mic fiu sau nepot
            int i = m;
            int left_son = i * 2;
            int right_son = i * 2 + 1;
            m = left_son;
            if (right_son <= dim)
                if (heap[m - 1] > heap[right_son - 1]) m = right_son;

            bool isGc = 0;  //vf daca e nepot
            if (left_son * 2 <= dim)
            {
                if (heap[m - 1] > heap[left_son * 2 - 1])
                {
                    m = left_son * 2;
                    isGc = 1;
                }
                if (left_son * 2 + 1 <= dim)
                    if (heap[m - 1] > heap[left_son * 2 + 1 - 1])
                    {
                        m = left_son * 2 + 1;
                        isGc = 1;
                    }
            }
            if (right_son * 2 <= dim)
            {
                if (heap[m - 1] > heap[right_son * 2 - 1])
                {
                    m = right_son * 2;
                    isGc = 1;
                }
                if (right_son * 2 + 1 <= dim)
                    if (heap[m - 1] > heap[right_son * 2 + 1 - 1])
                    {
                        m = right_son * 2 + 1;
                        isGc = 1;
                    }
            }

            if (isGc)
            {
                if (heap[m - 1] < heap[i - 1])
                {
                    swap(heap[m - 1], heap[i - 1]);
                    if (heap[m - 1] > heap[m / 2 - 1]) swap(heap[m - 1], heap[m / 2 - 1]);
                }
            }
            else if (heap[m - 1] < heap[i - 1]) swap(heap[m - 1], heap[i]);
        }
    }
    void pushDownMax(int m)
    {
        while (m * 2 <= dim)
        {
            int i = m;
            int left_son = i * 2;
            int right_son = i * 2 + 1;
            m = left_son;
            if (right_son <= dim)
                if (heap[m - 1] < heap[right_son - 1]) m = right_son;

            bool isGc = 0;
            if (left_son * 2 <= dim)
            {
                if (heap[m - 1] < heap[left_son * 2 - 1])
                {
                    m = left_son * 2;
                    isGc = 1;
                }
                if (left_son * 2 + 1 <= dim)
                    if (heap[m - 1] < heap[left_son * 2 + 1 - 1])
                    {
                        m = left_son * 2 + 1;
                        isGc = 1;
                    }
            }
            if (right_son * 2 <= dim)
            {
                if (heap[m - 1] < heap[right_son * 2 - 1])
                {
                    m = right_son * 2;
                    isGc = 1;
                }
                if (right_son * 2 + 1 <= dim)
                    if (heap[m - 1] < heap[right_son * 2 + 1 - 1])
                    {
                        m = right_son * 2 + 1;
                        isGc = 1;
                    }
            }

            if (isGc)
            {
                if (heap[m - 1] > heap[i - 1])
                {
                    swap(heap[m - 1], heap[i - 1]);
                    if (heap[m - 1] < heap[m / 2 - 1]) swap(heap[m - 1], heap[m / 2 - 1]);
                }
            }
            else if (heap[m - 1] > heap[i - 1]) swap(heap[m - 1], heap[i]);
        }
    }
    void pushUp(int i)
    {
        if (i != 1)
        {
            if (level(i) % 2 == 0)
            {
                if (heap[i - 1] > heap[i / 2 - 1])
                {
                    swap(heap[i - 1], heap[i / 2 - 1]);
                    pushUpMax(i / 2);
                }
                else pushUpMin(i);
            }
            else
            {
                if (heap[i - 1] < heap[i / 2 - 1])
                {
                    swap(heap[i - 1], heap[i / 2 - 1]);
                    pushUpMin(i / 2);
                }
                else pushUpMax(i);
            }
        }
    }
    void pushUpMin(int i)
    {
        if (i / 4 && heap[i - 1] < heap[i / 4 - 1])
        {
            swap(heap[i - 1], heap[i / 4 - 1]);
            i = i / 4;
        }
    }
    void pushUpMax(int i)
    {
        if (i / 4 && heap[i - 1] > heap[i / 4 - 1])
        {
            swap(heap[i - 1], heap[i / 4 - 1]);
            i = i / 4;
        }
    }
public:
    MinMaxHeap() : dim(0) {}
    void build(int n, vector <int> v)
    {
        dim = n;
        heap = v;
        for (int i = n / 2 + 1; i >= 1; i--) pushDown(i);
    }
    void insert(int x)
    {
        if (heap.size() > dim) heap[dim] = x;
        else heap.push_back(x);
        dim++;
        pushUp(dim);
    }
    void deleteMin()
    {
        if (dim == 1) dim = 0;
        else
        {
            heap[0] = heap[dim - 1];
            dim--;
            pushDown(1);
        }
    }
    void deleteMax()
    {
        if (dim < 3) dim--;
        else
        {
            int maxim;
            if (heap[1] > heap[2]) maxim = 2;
            else maxim = 3;
            heap[maxim - 1] = heap[dim - 1];
            dim--;
            pushDown(maxim);
        }
    }
    int printMin() const { return heap[0]; }
    int printMax() const
    {
        if (dim == 1) return heap[0];
        else if (dim == 2) return heap[1];
        else return max(heap[1], heap[2]);
    }
    int size() const { return dim; }
};

int main()
{
    MinMaxHeap heap;

    int n, operatie;
    fin >> n;
    while (n--)
    {
        fin >> operatie;
        if (operatie == 6)
        {
            int n, x;
            vector <int> v;
            fin >> n;
            for (int i = 1; i <= n; i++)
            {
                fin >> x;
                v.push_back(x);
            }
            heap.build(n, v);
        }
        else if (operatie == 1)
        {
            int x;
            fin >> x;
            heap.insert(x);
        }
        else if (operatie == 2)
        {
            if (!heap.size()) cout << "Heap-ul este gol!\n";
            else
            {
                heap.deleteMin();
                cout << "Minim sters\n";
            }
        }
        else if (operatie == 3)
        {
            if (!heap.size()) cout << "Heap-ul este gol!\n";
            else
            {
                heap.deleteMax();
                cout << "Maxim sters\n";
            }
        }
        else if (operatie == 4)
        {
            if (!heap.size()) cout << "Heap-ul este gol!\n";
            else cout << "Minimul: " << heap.printMin() << '\n';
        }
        else if (operatie == 5)
        {
            if (!heap.size()) cout << "Heap-ul este gol!\n";
            else cout << "Maximul: " << heap.printMax() << '\n';
        }
    }
    return 0;
}