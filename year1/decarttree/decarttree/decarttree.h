#ifndef DECARTTREE
#define DECARTTREE

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>

using std::max;
using std::cin;
using std::cout;
using std::endl;
using std::abs;
using std::vector;

typedef int DATA;

class CDecartTree
{
public:
    void insert(DATA element, int priority);
    void erase (DATA key);

    int getHeight();
    int getMaxWidth();

    CDecartTree();
private:
    class CNode;

    CNode *head;

};

class CDecartTree::CNode
{
public:
    DATA key;
    int  prior;

    CNode *left;
    CNode *right;

    CNode(DATA _key = 0, int _prior = 0, CNode * _left = NULL, CNode *_right = NULL);

    static CNode *merge (CNode *_left, CNode *_right);
    static void split (DATA key, CNode *_tree, CNode * &_left, CNode * &_right);
    static void insert(CNode * &_tree, DATA _key, int _prior);
    static void erase (CNode * &_tree, DATA _key);

    static int  getHeight(CNode *_tree);
    static void getMaxWidth(CNode *_tree, int level, vector<int> &h);
};

class SNaiveNode
{
public:
    DATA key;

    SNaiveNode *l;
    SNaiveNode *r;

    SNaiveNode (DATA _newkey)
    {
        key = _newkey;
        l = r = NULL;
    }

    static void insert(SNaiveNode *&_tree, DATA _key);

    static int  getHeight(SNaiveNode *_tree);
    static void getMaxWidth(SNaiveNode *_tree, int level, vector<int> &h);
};

class CNaiveTree
{
public:
    void insert(DATA element) {
        SNaiveNode::insert(head, element);
    };

    int getHeight();
    int getMaxWidth();

    CNaiveTree() : head(NULL) {}

private:
    SNaiveNode *head;
};

#endif // DECARTTREE

