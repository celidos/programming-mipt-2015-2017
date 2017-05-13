#ifndef SPLAYTREE
#define SPLAYTREE

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

typedef int DATA;

class SplayNode
{
public:
    DATA key;
    int  size;

    // functions
    DATA sum;

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
    static void        split (SplayNode *&v, int pos, SplayNode *&left, SplayNode *&right);
    static SplayNode * insert(SplayNode *&v, int pos, DATA key);
    static SplayNode * merge (SplayNode *left, SplayNode *right);
    static SplayNode * remove(SplayNode *&v, int pos);

    static int  getSize(SplayNode *v);
    static DATA getSum (SplayNode *v);

    static void update (SplayNode *v);

    static DATA reqSum (SplayNode *&v, int from, int to);

    static void printTree(SplayNode *v);
};

class SplayTree
{
public:
    SplayTree();
    ~SplayTree();

    void insert(int pos, DATA key);
    void remove(DATA key);
    DATA getElem(int pos);

    DATA getSum(int from, int to);
    void change(int pos, DATA newkey);

    void print();

private:
    SplayNode * head;
};

#endif // SPLAYTREE

