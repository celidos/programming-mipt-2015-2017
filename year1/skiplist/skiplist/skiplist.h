#ifndef SKIPLIST
#define SKIPLIST

#include <iostream>
#include <stack>
#include <ctime>
#include <cstdlib>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::stack;
using std::string;

typedef string DATA;

const DATA  INF = "}";
const DATA MINF = "_";

struct SNode
{
    DATA key;

    SNode * down;
    SNode * next;
    SNode * prev;

    SNode(DATA _key) : key(_key), down(NULL), next(NULL), prev(NULL) {}

    ~SNode()
    {
        if (next != NULL) delete next;
    }
};

class CSkipList
{
public:
    CSkipList();
    ~CSkipList();

    bool insert(DATA _key);
    bool erase (DATA _key);
    bool search(DATA _key);
    void printList();
private:
    bool searchKey(SNode *node, DATA &_key);

    bool insert(SNode *&node, DATA &_key);
    void addNodeAndPreviousLayers(SNode *&node, DATA &_key);
    void addNewLayer             ();

    bool eraseElement(SNode *&node, DATA &_key);

    int  random0or1();

    void initRandom();

    SNode *head;

    stack <SNode *> path;
};

#endif // SKIPLIST

