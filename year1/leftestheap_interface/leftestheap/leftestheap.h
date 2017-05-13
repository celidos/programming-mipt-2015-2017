#ifndef LEFTESTHEAP
#define LEFTESTHEAP

#include <iostream>
#include <algorithm> // swap
#include <cmath>     // min
#include <vector>

using std::swap;
using std::min;
using std::vector;
using std::cin;
using std::cout;
using std::endl;

struct SNode;
struct SLeftestHeap;

class IMergeableHeap
{
public:
    virtual void addHeap(int element)                 = 0;
    virtual void insert(int heapIndex, int key)       = 0;
    virtual void meld(int heapIndex1, int heapIndex2) = 0;
    virtual int  extractMin(int heapIndex)            = 0;
};

class CLeftestHeaps : public IMergeableHeap
{
public:
    void addHeap(int element);
    void insert(int heapIndex, int key);
    void meld(int heapIndex1, int heapIndex2);
    int  extractMin(int heapIndex);

private:
    vector <SLeftestHeap> heaps;
};

struct SNode
{
    int key;
    int dist;
    SNode *left;
    SNode *right;
    
    SNode (int _key, int _dist = 1)
    {
        key = _key;
        dist = _dist;
        left = NULL;
        right = NULL;
    }
};

struct SLeftestHeap
{
public:
    SLeftestHeap();
    SLeftestHeap(int element) {head = new SNode(element); cout << "done!" << endl; }
    ~SLeftestHeap();
    
    SLeftestHeap mergeWith(SLeftestHeap &heapB);
    SNode *merge(SNode *A, SNode *B);
    void insert(int element);
    int extractMin();

    bool empty();

    void freemem(SNode *node);

    int dist(SNode *node);
private:
    SNode *head;
};

#endif // LEFTESTHEAP
