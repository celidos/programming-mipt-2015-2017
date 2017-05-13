/*
 * Биномиальная куча, версия 9 дек. 2015
 *
 * Структура Биномиальная куча - это вектор из корней биномиальных деревьев
 *
 *  [   O   ]>[   O   ]>[   O   ]>[   O   ]> ...
 *     / \       / \       / \       / \
 *    o   o     o   o     o   o     o   o
 *   / \ / \   / \ / \   / \ / \   / \ / \
 *
 *  Биномиальное дерево - это указатель на структуру SNode, являющуюся корнем
 */

#ifndef BINHEAP_INC
#define BINHEAP_INC

#include <iostream>
#include <vector>
#include <cstdlib> // swap
#include <cmath>   // min
#include <string>

using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::min;
using std::swap;
using std::string;

typedef int DATA;

const DATA INF = 1234500000;

class IMergeableHeap
{
public:
    virtual void addHeap   (DATA element)                   = 0;
    virtual void insert    (int heapIndex,  DATA key)       = 0;
    virtual void meld      (int heapIndex1, int heapIndex2) = 0;
    virtual DATA extractMin(int heapIndex)                  = 0;
};

class CHeapsManager : public IMergeableHeap
{
private:
    class CBinHeap;
    vector <CBinHeap> heaps;

public:
    void addHeap   (DATA element);
    void insert    (int heapIndex,  DATA key);
    void meld      (int heapIndex1, int heapIndex2);
    DATA extractMin(int heapIndex);

    void _printAll();

    ~CHeapsManager();
};

class CHeapsManager::CBinHeap
{
public:
    struct SNode;

    // ----------------------
    vector <SNode *> heads;

    // ----------------------
    CBinHeap(DATA element);
    CBinHeap() {}
    ~CBinHeap() {}

    // ----------------------
    CBinHeap mergeWith(CBinHeap &bheap);
    void insert(DATA element);
    DATA extractMin();
    bool empty();

    void _printAll();
    void _printNode(SNode *n);

private:
    void cleanTrash();
    int  searchHeapIndexWithMin();
};

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

    static SNode *merge(SNode *a, SNode *b);
};

#endif
