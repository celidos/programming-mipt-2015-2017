#ifndef AVLTREE
#define AVLTREE

#include <iostream>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::min;
using std::max;
using std::abs;

typedef int DATA;

struct SNode
{
    int key;
    int h;

    int treeSize;

    SNode* left;
    SNode* right;

    SNode(DATA _key) :
        key(_key), left(NULL), right(NULL), h(1), treeSize(1) {}

    ~SNode()
    {
        if (left  != NULL) delete left;
        if (right != NULL) delete right;
    }
};

class CAVLtree
{
public:
    CAVLtree();
    ~CAVLtree();

    void insert  (DATA _key) { head = insert(head, _key); }
    void erase   (DATA _key) { head = remove(head, _key); }
    DATA getKstat(int  k)    { return getKstat(head, k + 1); }

    void printTree();

private:
    SNode *head;

    int getHeight(SNode* tree);

    int getBalance(SNode* tree);
    int getTreeSize(SNode* tree);

    void refreshHeight(SNode* tree);
    void refreshTreeSize(SNode* tree);

    SNode* rotateRight(SNode* tree);
    SNode* rotateLeft (SNode* tree);

    SNode* refreshBalance  (SNode* tree);

    SNode* insert         (SNode* tree, DATA key);
    SNode* searchMinNode  (SNode* tree);
    SNode* removeMin      (SNode* tree);
    SNode* remove         (SNode* tree, DATA key);

    DATA   getKstat       (SNode* tree, int k);

    void printNode(SNode * tree);
};

#endif // AVLTREE

