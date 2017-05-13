#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::swap;

typedef unsigned int uint;

template< typename T >
void HeapSiftDown(T *heap, uint heapsize, uint index)
{
    uint tochange = index;
    do
    {
        uint lchild = 2*index + 1, rchild = 2*index + 2;
        if (lchild < heapsize)
            if (heap[lchild] > heap[index])
                tochange = lchild;

        if (rchild < heapsize)
            if (heap[rchild] > heap[index])
                tochange = (heap[tochange] < heap[rchild] ? rchild : tochange);

        swap(heap[tochange], heap[index]);

        if (index == tochange)
            break;
        index = tochange;
    }
    while (true);
}

template< typename T >
void HeapBuild(T *heap, uint heapsize)
{
    if (heapsize <= 1)
        return;

    for (int i = heapsize; i >= 0; --i)
        HeapSiftDown(heap, heapsize, i);
}

template< typename T >
void HeapSort(T *heap, uint heapsize)
{
    if (heapsize <= 1)
        return;

    HeapBuild(heap, heapsize);

    for (int i = 0; i < (int)heapsize - 1; ++i)
    {
        swap(heap[0], heap[heapsize - i - 1]);
        HeapSiftDown(heap, heapsize - i - 1, 0);
    }
}

enum SegmentPart {START, FINISH};

struct Segment
{
    int x;
    SegmentPart type;
    Segment() {};
    Segment(int X, SegmentPart T) : x(X), type(T) {}
    bool operator> (Segment &b)
    {
        return this->x > b.x;
    }
    bool operator< (Segment &b)
    {
        return this->x < b.x;
    }
};

int main()
{
    uint N;
    do
    {
        cin >> N;
    } while (!N);
    Segment *sArray = new Segment[2*N];
    for (uint i = 0; i < N; ++i)
    {
        cin >> sArray[2 * i].x >> sArray[2 * i + 1].x;
        sArray[2 * i    ].type = START;
        sArray[2 * i + 1].type = FINISH;
    }

    HeapSort(sArray, 2*N);

    int balance = 1, lastx = sArray[0].x;
    int sum = 0;
    for (uint i = 1; i < 2*N; ++i)
    {
        if (balance == 1)
            sum += sArray[i].x - lastx;

        if (sArray[i].type == START)
            ++balance;
        else
            --balance;

        lastx = sArray[i].x;
    }

    cout << sum << endl;

    return 0;
}
