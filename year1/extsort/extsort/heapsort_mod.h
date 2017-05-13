#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <cstdlib>
#include <iostream>

typedef unsigned int uint;

using std::swap;
using std::cin;
using std::cout;
using std::endl;

template< typename T >
void HeapSiftDown(T *heap, const uint heapsize, uint index)
{
    uint tochange = index;
    do
    {
        uint lchild = 2*index + 1, rchild = 2*index + 2;
        if (lchild < heapsize)
            if (heap[lchild] < heap[index]) // change to min/max switch
                tochange = lchild;

        if (rchild < heapsize)
            if (heap[rchild] < heap[index]) // change to min/max switch
                tochange = (heap[rchild] < heap[tochange] ? rchild : tochange); // change to min/max switch

        swap(heap[tochange], heap[index]);

        if (index == tochange)
            break;
        index = tochange;
    }
    while (true);
}

template< typename T >
void HeapBuild(T *heap, const uint heapsize)
{
    if (heapsize <= 1)
        return;

    for (int i = heapsize; i >= 0; --i)
        HeapSiftDown(heap, heapsize, i);
}

template< typename T >
void HeapSort(T *heap, const uint heapsize)
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

template< typename T >
T ExtractMin(T *heap, uint heapsize)
{
    //assert( !arr.IsEmpty() );
    T result;
    if (heapsize > 0)
        result = heap[0];
    else
    {
        cout << "Heap: ExtractMin: Error - heap is empty" << endl;
    }

    // Переносим последний элемент в корень.

    heap[0] = heap[heapsize - 1];

    if (heapsize > 0)
        HeapSiftDown(heap, heapsize - 1, 0);

    return result;
}

#endif // HEAPSORT_H

