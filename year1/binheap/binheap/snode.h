#ifndef SNODE
#define SNODE

#include <binheap.h>

struct CHeapsManager::CBinHeap::SNode
{
    DATA key;
    int  deg;
    SNode *child;   // левое дите
    SNode *bro;     // правый брат

    SNode(DATA _key, int _degree)
    {
        key = _key;
        deg = _degree;
        child = bro = NULL;
    }

    ~SNode()
    {
        if (child != NULL) delete child;
        if (bro   != NULL) delete bro;
    }
};

#endif // SNODE

