#include <leftestheap.h>

// CleftestHeap --------------------------------------------------

CHeapsManager::CLeftestHeap::CLeftestHeap()
{
    head = NULL;
}

CHeapsManager::CLeftestHeap::CLeftestHeap(DATA element)
{
    head = new SNode(element);
}

CHeapsManager::CLeftestHeap CHeapsManager::CLeftestHeap::mergeWith(CLeftestHeap &heapB)
{
    CLeftestHeap newheap;
    newheap.head = merge(head, heapB.head);
    return newheap;
}

CHeapsManager::CLeftestHeap::SNode * CHeapsManager::CLeftestHeap::merge(SNode *A, SNode *B)
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

void CHeapsManager::CLeftestHeap::insert(DATA element)
{
    CLeftestHeap newheap(element);
    head = merge(head, newheap.head);
}

DATA CHeapsManager::CLeftestHeap::extractMin()
{
    DATA result = head->key;
    head = merge(head->left, head->right);

    return result;
}

bool CHeapsManager::CLeftestHeap::empty()
{
    return (head == NULL);
}

int  CHeapsManager::CLeftestHeap::dist(SNode *node)
{
    if (node == NULL)
        return 0;

    if (node->right == NULL || node->left == NULL)
        return 1;

    return min(node->left->dist, node->right->dist) + 1;
}

void CHeapsManager::CLeftestHeap::_printAll()
{
    cout << "[ ";
    _printNode(this->head);
    cout << "]" << endl;
}

void CHeapsManager::CLeftestHeap::_printNode(SNode *node)
{
    if (node == NULL)
    {
        cout << "-, ";
        return;
    }

    cout << node->key << ", ";

    _printNode(node->left);
    _printNode(node->right);
}

// CHeapsManager --------------------------------------------------

void CHeapsManager::addHeap(DATA element)
{
    heaps.push_back(CLeftestHeap(element));
}

void CHeapsManager::insert(int heapIndex, DATA key)
{
    heaps[heapIndex].insert(key);
}

void CHeapsManager::meld(int heapIndex1, int heapIndex2)
{
    heaps[heapIndex1] = heaps[heapIndex1].mergeWith(heaps[heapIndex2]);
    heaps[heapIndex2] = heaps[heaps.size() - 1];
    heaps.pop_back();
}

DATA CHeapsManager::extractMin(int heapIndex)
{
    DATA minimum = heaps[heapIndex].extractMin();

    if (heaps[heapIndex].empty())
    {
        heaps[heapIndex] = heaps[heaps.size() - 1];
        heaps.pop_back();
    }

    return minimum;
}

void CHeapsManager::_printAll()
{
    cout << "------" << endl;
    for (int i = 0; i < heaps.size(); ++i)
    {
        cout << "HEAP #" << i << " : ";
        heaps[i]._printAll();
    }
    cout << "~~~~~~" << endl;
}

CHeapsManager::~CHeapsManager()
{
    for (int i = 0; i < heaps.size(); ++i)
        CLeftestHeap::SNode::freemem( heaps[i].head );
}
