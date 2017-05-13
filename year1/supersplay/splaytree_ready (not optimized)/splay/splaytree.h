#ifndef SPLAYTREE
#define SPLAYTREE

#include <iostream>

#define INF 1234500000

using std::cin;
using std::cout;
using std::endl;
using std::swap;

typedef int DATA;

class SplayNode
{
public:
    DATA key;
    int  size;

    DATA sum;
    DATA add;
    bool rev;

    int maxLeft;
    int maxRight;
    DATA leftKey;
    DATA rightKey;

    SplayNode *parent;
    SplayNode *left;
    SplayNode *right;

    SplayNode(DATA _key, SplayNode *_left = nullptr, SplayNode *_right = nullptr,
               SplayNode *_parent = nullptr);
    ~SplayNode();

    static void keepParent(SplayNode *node);

    static void setParent(SplayNode *child, SplayNode *newparent);
    static void rotate   (SplayNode *parent, SplayNode *child);

    SplayNode * splay();

    static SplayNode * find  (SplayNode *v,  int pos);
    static SplayNode * findKeyInDecreaseSegment(SplayNode *v, DATA key);
    static void        split (SplayNode *&v, int pos, SplayNode *&left, SplayNode *&right);
    static SplayNode * insert(SplayNode *&v, int pos, DATA key);
    static SplayNode * merge (SplayNode *left, SplayNode *right);
    static SplayNode * remove(SplayNode *&v, int pos);

    static int  getSize(SplayNode *v);
    static DATA getSum (SplayNode *v);
    static void useAdd (SplayNode *v, DATA add);
    static void useRev (SplayNode *v);

    static void update (SplayNode *v);
    static void pushAdd(SplayNode *v);
    static void pushRev(SplayNode *v);

    static DATA reqSum (SplayNode *&v, int from, int to);
    static void addRange(SplayNode *&v, int from, int to, DATA addition);
    static void revRange(SplayNode *&v, int from, int to);

    static void swapRoots(SplayNode *&v, SplayNode *&u);

    static void nextPermutation(SplayNode *&v, int from, int to);

    static void printTree(SplayNode *v, int level);
    static void printArray(SplayNode *v);
};

class SplayTree
{
public:
    SplayTree();
    ~SplayTree();

    void insert(int pos, DATA key);
    void remove(DATA key);
    DATA getElem(int pos);

    DATA getSum (int from, int to);
    void change (int pos, DATA newkey);

    void add    (int from, int to, DATA addition);
    void reverse(int from, int to);
    void nextPermutation(int from, int to);

    void print();
    void printAsArray();

private:
    SplayNode * head;
};

#endif // SPLAYTREE
