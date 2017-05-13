#include <leftestheap.h>

SLeftestHeap::SLeftestHeap()
{
    head = NULL;
}

SLeftestHeap::~SLeftestHeap()
{
    freemem(head);
}

SLeftestHeap SLeftestHeap::mergeWith(SLeftestHeap &heapB)
{
    SLeftestHeap newheap;
    newheap.head = merge(head, heapB.head);
    return newheap;
}

SNode * SLeftestHeap::merge(SNode *A, SNode *B)
{
    if (A == NULL)
        return B;

    if (B == NULL)
        return A;

    if (A->key > B->key)
        swap(A, B);

    A->right = merge(A->right, B);

    if (dist(A->right) > dist(A->left))
        swap(A->left, A->right);

    A->dist = dist(A);
    
    return A;
}

void SLeftestHeap::insert(int element)
{
    SLeftestHeap newheap(element);
    head = merge(head, newheap.head);
}

int SLeftestHeap::extractMin()
{
    int result = head->key;
    head = merge(head->left, head->right);

    return result;
}

bool SLeftestHeap::empty()
{
    return (head == NULL);
}

void SLeftestHeap::freemem(SNode *node)
{
    if (node == NULL)
        return;

    freemem(node->left);
    freemem(node->right);

    delete node;
}

int SLeftestHeap::dist(SNode *node)
{
    if (node == NULL)
        return 0;

    if (node->right == NULL || node->left == NULL)
        return 1;

    return min(node->left->dist, node->right->dist) + 1;
}


void CLeftestHeaps::addHeap(int element)
{
    cout << "ok" << endl;
    SLeftestHeap newheap(element);

    cout << "just ok" << endl;
    heaps.push_back(newheap);
    cout << "still ok!" << endl;
}

void CLeftestHeaps::insert(int heapIndex, int key)
{
    cout << "just ok " << endl;
    heaps[heapIndex].insert(key);
}

int CLeftestHeaps::extractMin(int heapIndex)
{
    int res = heaps[heapIndex].extractMin();
    if (heaps[heapIndex].empty())
    {
        heaps[heapIndex] = heaps[heaps.size() - 1];
        heaps.pop_back();
    }

    return res;
}

void CLeftestHeaps::meld(int heapIndex1, int heapIndex2)
{
    heaps[heapIndex1] = heaps[heapIndex1].mergeWith(heaps[heapIndex2]);
    heaps[heapIndex2] = heaps[heaps.size() - 1];
    heaps.pop_back();
}

