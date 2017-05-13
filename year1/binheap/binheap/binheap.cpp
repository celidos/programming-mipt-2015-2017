#include <binheap.h>


CHeapsManager::CBinHeap CHeapsManager::CBinHeap::mergeWith(CBinHeap &bheap)
{
    CBinHeap &A = (*this);
    CBinHeap &B = bheap;

    int heads1size = A.heads.size();
    int heads2size = B.heads.size();

    CBinHeap H;
    SNode *p = NULL;

    // Сливаем общую часть ---------------------------------------------------

    for (int i = 0; i < min(heads1size, heads2size); ++i)
    {
        int cases = 0;
        if (p != NULL)          ++cases;
        if (A.heads[i] != NULL) ++cases;
        if (B.heads[i] != NULL) ++cases;

        if (cases < 2)
        {
            H.heads.push_back(  merge( merge(A.heads[i], B.heads[i]), p )  );
            p = NULL;
        }
        else if (cases == 2)
        {
            p = merge( merge(A.heads[i], B.heads[i]), p );
            H.heads.push_back(NULL);
        }
        else
        {
            H.heads.push_back(p);
            p = merge(A.heads[i], B.heads[i]);
        }
    }

    // Хвост --------------------------------------------------------------

    for (int i = min(heads1size, heads2size); i < heads1size; ++i)
    {
        if (p != NULL && A.heads[i] != NULL)
        {
            H.heads.push_back(NULL);
            p = merge(p, A.heads[i]);
        }
        else
        {
            H.heads.push_back(merge(p, A.heads[i]));
            p = NULL;
        }
    }

    for (int i = min(heads1size, heads2size); i < heads2size; ++i)
    {
        if (p != NULL && B.heads[i] != NULL)
        {
            H.heads.push_back(NULL);
            p = merge(p, B.heads[i]);
        }
        else
        {
            H.heads.push_back(merge(p, B.heads[i]));
            p = NULL;
        }
    }

    if (p != NULL)
         H.heads.push_back(p);

    // -------------------------------------------------------------------

    cleanTrash();

    return H;
}

void CHeapsManager::CBinHeap::insert(DATA element)
{
    CBinHeap newheap(element);
    *this = this->mergeWith(newheap);
}

DATA CHeapsManager::CBinHeap::extractMin()
{
    // Вытащим вершину-минимум ------------------------------------------------

    int minindex = searchHeapIndexWithMin();

    SNode *minNode = heads[minindex];
    heads[minindex] = NULL;

    // Склеим ее детей в новую кучу -------------------------------------------

    CBinHeap newheap;
    newheap.heads = vector <SNode *> (minNode->deg + 1, (SNode *)NULL);

    SNode *iter = minNode->child;
    while (iter != NULL)
    {
        newheap.heads[iter->deg] = iter;
        iter = iter->bro;
    }

    // Объединим со старой кучей ----------------------------------------------

    *this = this->mergeWith(newheap);

    // Освободим память -------------------------------------------------------

    DATA minimum = minNode->key;
    minNode->bro = minNode->child = NULL; // Обязательно! иначе "delete minnode" удалит своих потомков
    delete minNode;

    cleanTrash();

    return minimum;
}

bool CHeapsManager::CBinHeap::empty()
{
    return (heads.size() == 0);
}

void CHeapsManager::CBinHeap::_printAll()
{
    for (int i = 0; i < heads.size(); ++i)
    {
        cout << "[ ";
        _printNode(heads[i]);
        cout << "] ";
    }
}

void CHeapsManager::CBinHeap::_printNode(SNode *n)
{
    if (n == NULL)
        return;

    cout << n->key << ", ";

    SNode *iter = n->child;
    while (iter != NULL)
    {
        _printNode(iter);
        iter = iter->bro;
    }
}

void CHeapsManager::CBinHeap::cleanTrash()
{
    // Освобождает пустые разряды в heads

    int iter = this->heads.size() - 1;
    while (this->heads[iter] == NULL && iter >= 0)
    {
        heads.pop_back();
        --iter;
    }
}

int  CHeapsManager::CBinHeap::searchHeapIndexWithMin()
{
    DATA min = INF;
    int  minindex = -1;

    for (int i = 0; i < (int)heads.size(); ++i)
    {
        if (heads[i] != NULL)
            if (heads[i]->key < min)
            {
                min = heads[i]->key;
                minindex = i;
            }
    }

    return minindex;
}

     CHeapsManager::CBinHeap::CBinHeap(DATA element)
{
    heads.push_back(new SNode(element, 0));
}


