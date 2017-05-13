#ifndef SPLAYTREE
#define SPLAYTREE

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

typedef int DATA;

class SplayTree
{
public:
    DATA key;

    SplayTree *parent;
    SplayTree *left;
    SplayTree *right;

    SplayTree(DATA _key, SplayTree *_left = nullptr, SplayTree *_right = nullptr,
               SplayTree *_parent = nullptr);
    ~SplayTree();

    static void keepParent(SplayTree *node);

    static void setParent(SplayTree *child, SplayTree *newparent);
    static void rotate   (SplayTree *parent, SplayTree *child);

    SplayTree * splay();

    static SplayTree * find  (SplayTree *v, DATA key);
    static void split (SplayTree *&v, DATA key, SplayTree *&left, SplayTree *&right);
    static SplayTree * insert(SplayTree *&v, DATA key);
    static SplayTree * merge (SplayTree *left, SplayTree *right);
    static SplayTree * remove(SplayTree *&v, DATA key);

    static void printTree(SplayTree *v);
private:

};

#endif // SPLAYTREE

