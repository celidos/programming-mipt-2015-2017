/*
 * Левацкая куча, версия 9 дек. 2015
 */

#ifndef LEFTESTHEAP
#define LEFTESTHEAP

#include <iostream>
#include <algorithm> // swap
#include <cmath>     // min
#include <vector>
#include <stack>

using std::swap;
using std::min;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::stack;

typedef int DATA;

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
    class CLeftestHeap;
    vector <CLeftestHeap> heaps;

public:
    void addHeap   (DATA element);
    void insert    (int heapIndex,  DATA key);
    void meld      (int heapIndex1, int heapIndex2);
    DATA extractMin(int heapIndex);

    void _printAll();

    ~CHeapsManager();
};

class CHeapsManager::CLeftestHeap
{
private:
    friend class CHeapsManager;
    struct SNode;

    SNode *head;

    static SNode* merge(SNode *A, SNode *B);
    static int dist(SNode *node);
public:
    CLeftestHeap();
    CLeftestHeap(DATA element);
    ~CLeftestHeap() {}
    
    // -----------
    CLeftestHeap mergeWith(CLeftestHeap &heapB);
    void insert(DATA element);
    DATA extractMin();
    bool empty();

    // -----------
    void _printAll();
    void _printNode(SNode * node);
};

struct CHeapsManager::CLeftestHeap::SNode
{
    DATA key;
    int  dist;
    SNode *left;
    SNode *right;

    SNode (DATA _key)
    {
        key = _key;
        dist = 1;
        left = NULL;
        right = NULL;
    }

    ~SNode()
    {
        //if (left  != NULL) delete left;
        //if (right != NULL) delete right;
    }

    static void freemem(SNode * node)
    {
        stack <SNode *> st;
        st.push(node);

        SNode * p;
        while (!st.empty())
        {
            p = st.top();
            st.pop();

            if (p != NULL)
            {
                if (p->left  != NULL) st.push(p->left);
                if (p->right != NULL) st.push(p->right);

                delete p;
            }
        }
    }
};


#endif // LEFTESTHEAP
