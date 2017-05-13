#include <iostream>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;
using std::swap;

typedef unsigned int uint;

template< class T >
class MyVector
{
public:
    void push_back(const T &element);
    T    at(uint index);
    T    set(int index, const T &newElement);
    int  getsize() {return size;}

    MyVector();
    MyVector(int Size);
    ~MyVector();
    T *data;
private:
    uint size;
    uint maxSize;
};

template<typename T>
MyVector<T>::MyVector() :
    size(0), maxSize(1)
{
    data = new T[maxSize];
}

template<typename T>
MyVector<T>::MyVector(int Size) :
    size(0), maxSize(Size)
{
    data = new T[maxSize];
}

template<typename T>
MyVector<T>::~MyVector()
{
    if (data)
        delete [] data;
}

template<typename T>
void MyVector<T>::push_back(const T &element)
{
    if (size >= maxSize && data)
    {
        T *newdata = new T[2*maxSize];
        memcpy(newdata, data, maxSize * sizeof(T));
        delete [] data;
        data = newdata;
        maxSize *= 2;
    }

    data[size] = element;
    ++size;
}

template<typename T>
T MyVector<T>::at(uint index)
{
    if (index < size)
        return data[index];
    else
        cout << "Error! Invalid index at 'at'." << endl;
}

template<typename T>
T MyVector<T>::set(int index, const T &newElement)
{
    if (index < size)
    {
        data[index] = newElement;
        return data[index];
    }
    else
        cout << "Error! Invalid index at 'set'." << endl;
}


template <typename T>
int _internal_merge_sort(T *A, int l, int r)
{
    int SummaryAnswer = 0;
    if (r - l == 2)
    {
        if (A[l] > A[l+1])
        {
            swap(A[l], A[l+1]);
            ++SummaryAnswer;
        }
    }
    else if (r - l > 2)
    {
        int m = (l + r) / 2;

        SummaryAnswer += _internal_merge_sort(A, l, m);
        SummaryAnswer += _internal_merge_sort(A, m, r);

        T *buffer = new T[r - l];

        int leftIndex = l, rightIndex = m, index = 0;
        while (leftIndex < m && rightIndex < r)
        {
            if (A[leftIndex] > A[rightIndex])
            {
                buffer[index] = A[rightIndex];
                ++rightIndex;
                SummaryAnswer += m - leftIndex;
            }
            else
            {
                buffer[index] = A[leftIndex];
                ++leftIndex;
            }
            ++index;

            if (leftIndex == m)
            {
                for (int i = rightIndex; i < r; ++i)
                    buffer[index++] = A[i];
            }
            if (rightIndex == r)
            {
                for (int i = leftIndex; i < m; ++i)
                    buffer[index++] = A[i];
            }
        }

        for (int i = 0; i < r - l; i++)
            A[l + i] = buffer[i];

        delete [] buffer;
    }
    return SummaryAnswer;
}

template <typename T>
int MergeSortAndFindInv(MyVector <T> Array)
{
    _internal_merge_sort(Array.data, 0, Array.getsize());
}


int main()
{
    MyVector <int> A;

    int t;
    while (cin >> t)
    {
        A.push_back(t);
    }

    cout << MergeSortAndFindInv(A) << endl;

    return 0;
}

