#ifndef SEGMENTTREE
#define SEGMENTTREE

#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::min;
using std::max;

const int INF = 1234500000;

typedef int DATA;

struct SNode
{
    DATA key;
    DATA newcolor;

    SNode(DATA _key = 0, DATA _newcolor = -1) :
        key(_key), newcolor(_newcolor)
    {}
};

class CSegmentTree
{
public:
    CSegmentTree(int _size);
    ~CSegmentTree();

    void getDataFromKeyboard();

    void build  ()                               { build (0, 0, dataSize);                        }
    void replace(int _index, DATA _with)         { update(0, 0, dataSize, _index, _with);         }
    DATA getmin (int _from, int _to)             { return getmin(0, 0, dataSize, _from, _to + 1); }
    void fill   (int _from, int _to, DATA _with) { fill  (0, 0, dataSize, _from, _to + 1, _with); }

private:
    void build    (int v, int left, int right);
    void update   (int v, int left, int right, int to_update, DATA color);
    DATA getmin   (int v, int left, int right, int from, int to);
    void fill     (int v, int left, int right, int from, int to, DATA with);
    void push     (int v);

    DATA* data;
    int  dataSize;

    SNode* tree;
    int    treeSize;
};



#endif // SEGMENTTREE

