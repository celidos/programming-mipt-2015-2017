#ifndef SPLAYTREE
#define SPLAYTREE

#include <iostream>

#include <array_interface.h>

#define INF 1234500000

using std::cin;
using std::cout;
using std::endl;
using std::swap;

typedef int DATA;

class SplayNode
{
private:
    // data
    DATA key;
    size_t size;      // of subtree

    // relatives
    SplayNode *parent;
    SplayNode *left;
    SplayNode *right;

    // functions
    DATA sum;       // sum on the range
    DATA add;       // addition on the range
    bool rev;       // reverse flag

    // service for NP
    size_t maxLeft;
    size_t maxRight;
    DATA leftKey;
    DATA rightKey;

    // METHODS ----------------------------------------

    static void keepParent(SplayNode *v);                        // update children of parents with care
    static void setParent (SplayNode *child, SplayNode *newparent);

    // service
    static void        rotate (SplayNode *parent, SplayNode *child);    // rotate around parent-child edge
    SplayNode *        splay  ();
    static void        split  (SplayNode *&v, size_t pos, SplayNode *&left, SplayNode *&right);
    static SplayNode * merge  (SplayNode *left, SplayNode *right);

    static size_t      getSize(SplayNode *v); // with care
    static DATA        getSum (SplayNode *v); // with care
    static void        useAdd (SplayNode *v, DATA add);
    static void        useRev (SplayNode *v);

    static void        pushAddRev(SplayNode *v); // (pre-call)
    static void        update    (SplayNode *v); // updating sum, size and info for NP (post-call)

    static void        ejectSegment(SplayNode *v, size_t from, size_t to,
                                    SplayNode *&prePart, SplayNode *&segment, SplayNode *&postPart);

    // only for decreasing segment in nextPermutation
    static SplayNode * findPrevInDecreaseSegment    (SplayNode *v);   // previous node in decr. order
    static SplayNode * findMinInSubtree             (SplayNode *v);
    static SplayNode * findKeyInDecreaseSegment     (SplayNode *v, DATA key);

public:
    SplayNode(DATA _key, SplayNode *_left = nullptr, SplayNode *_right = nullptr,
              SplayNode *_parent = nullptr);

    ~SplayNode();

    DATA getKey();

    static SplayNode * findByPos(SplayNode *v , size_t pos);
    static SplayNode * insert   (SplayNode *&v, size_t pos, DATA key);
    static SplayNode * remove   (SplayNode *&v, size_t pos);

    static DATA reqSum  (SplayNode *&v, size_t from, size_t to);
    static void addRange(SplayNode *&v, size_t from, size_t to, DATA addition);
    static void revRange(SplayNode *&v, size_t from, size_t to);
    static void nextPermutation(SplayNode *&v, size_t from, size_t to);

    static void printTree(SplayNode *v, int level);
    static void printArray(SplayNode *v);
};

class SplayTree : public IArray
{
public:
    SplayTree();
    virtual ~SplayTree();

    virtual void insert (size_t pos, DATA key);
    virtual void remove (size_t pos);
    virtual DATA getElem(size_t pos);
    virtual DATA sum    (size_t from, size_t to);
    virtual void change (size_t pos, DATA newkey);
    virtual void add    (size_t from, size_t to, DATA addition);
    virtual void reverse(size_t from, size_t to);
    virtual void nextPermutation(size_t from, size_t to);

    virtual void print();

    void printAsTree();
private:
    SplayNode * head;
};

#endif // SPLAYTREE
