#include <splaytree.h>

// SPLAY NODE -----------------------------------------------------------------------------

SplayNode::SplayNode(DATA _key, SplayNode *_left, SplayNode *_right, SplayNode *_parent) :
    key(_key), size(1), parent(_parent), left(_left), right(_right),

    sum(_key), add(0), rev(false),

    maxLeft(1), maxRight(1), leftKey(_key), rightKey(_key)
{ }

SplayNode::~SplayNode()
{
    // recursively free
    if (left != nullptr)
        delete left;
    if (right != nullptr)
        delete right;
}

void SplayNode::keepParent(SplayNode *v)
{
    pushAddRev(v);

    setParent(v->left,  v);
    setParent(v->right, v);

    update(v);
}

void SplayNode::setParent(SplayNode *child, SplayNode *newparent)
{
    if (child != nullptr)
    {
        child->parent = newparent;
        update(newparent);
    }
}

void SplayNode::rotate(SplayNode *parent, SplayNode *child)
{
    SplayNode *grandpa = parent->parent;

    pushAddRev(grandpa);
    pushAddRev(parent);
    pushAddRev(child);

    if (grandpa != nullptr)
    {
        if (grandpa->left == parent)
            grandpa->left = child;
        else
            grandpa->right = child;
    }

    if (parent->left == child)
    {
        parent->left  = child->right;       // order is important!
        child ->right = parent;
    }
    else
    {
        parent->right = child->left;        // order is important!
        child ->left  = parent;
    }

    keepParent(child);
    keepParent(parent);

    update(parent);                         //
    update(child);                          // order is important!
    update(grandpa);                        //

    child->parent = grandpa;
}

SplayNode * SplayNode::splay()
{
    if (this->parent == nullptr)
        return this;

    SplayNode *parent  = this->parent;
    SplayNode *grandpa = parent->parent;

    pushAddRev(grandpa);
    pushAddRev(parent);
    pushAddRev(this);

    if (grandpa == nullptr) // Zig
    {
        rotate(parent, this);
        return this;
    }
    else
    {
        bool needZigZig = (grandpa->left == parent) == (parent->left == this);

        if (needZigZig) // ZigZig
        {
            rotate(grandpa, parent);
            rotate(parent, this);
        }
        else // ZigZag
        {
            rotate(parent, this);
            rotate(grandpa, this);
        }
        return this->splay();
    }
}

void SplayNode::split(SplayNode *&v, size_t pos, SplayNode *&left, SplayNode *&right)
{
    pushAddRev(v);

    if (v == nullptr)
    {
        left  = nullptr;
        right = nullptr;
        return;
    }

    if (pos < getSize(v)) // or we have to do nothing
    {
        v = findByPos(v, pos);

        SplayNode *newleft, *newright;

        newleft  = v->left;             // order is important, be careful
        setParent(newleft, nullptr);
        newright = v;
        v->left = nullptr;

        update(newright);

        left  = newleft;
        right = newright;
    }
    else
    {
        left = v;
        right = nullptr;
    }
}

SplayNode *SplayNode::merge(SplayNode *left, SplayNode *right)
{
    pushAddRev(left);
    pushAddRev(right);

    if (right == nullptr)
        return left;
    if (left == nullptr)
        return right;

    right = findByPos(right, 0);
    left->parent = right;               // order is important!
    right->left  = left;                //


    update(right->left);
    update(right->right);
    update(right);

    return right;
}

size_t SplayNode::getSize(SplayNode *v)
{
    if (v != nullptr)
        return v->size;
    return 0;
}

DATA SplayNode::getSum(SplayNode *v)
{
    if (v != nullptr)
    {
        pushAddRev(v);
        return v->sum;
    }

    return 0;
}

void SplayNode::useAdd(SplayNode *v, DATA add)
{
    if (v != nullptr)
        v->add += add;
}

void SplayNode::useRev(SplayNode *v)
{
    if (v != nullptr)
        v->rev = !v->rev;
}

void SplayNode::pushAddRev(SplayNode *v)
{
    if (v != nullptr)
    {
        // pushing 'add'
        v->key += v->add;
        v->sum += v->add * getSize(v);
        v->leftKey  += v->add;
        v->rightKey += v->add;

        useAdd(v->left,  v->add);
        useAdd(v->right, v->add);

        v->add = 0;

        // pushing 'rev'
        if (v->rev)
        {
            useRev(v);
            swap(v->maxLeft, v->maxRight);
            swap(v->leftKey, v->rightKey);
            swap(v->left,    v->right);
            useRev(v->left);
            useRev(v->right);
            update(v);
        }
    }
}

void SplayNode::update(SplayNode *v)
{
    if (v != nullptr)
    {
        pushAddRev(v);
        bool leftExist  = (v->left != nullptr),
             rightExist = (v->right != nullptr);

        // size and sum
        v->size = 1      + getSize(v->left) + getSize(v->right);
        v->sum  = v->key + getSum (v->left) + getSum (v->right);

        // subrange
        v->leftKey  = (leftExist  ? v->left ->leftKey  : v->key);
        v->rightKey = (rightExist ? v->right->rightKey : v->key);

        // left corner -----------------------------------------------
        {
            DATA lastkey = -INF;
            v->maxLeft   = 0;
            bool isok    = true;

            if (leftExist)
            {
                v->maxLeft = v->left->maxLeft;
                lastkey = v->left->rightKey;

                if (v->left->maxLeft < getSize(v->left))
                    isok = false;
            }

            if (isok && lastkey <= v->key)
            {
                lastkey = v->key;
                v->maxLeft++;

                if (rightExist)
                    if (lastkey <= v->right->leftKey)
                    {
                        v->maxLeft += v->right->maxLeft;
                        v->maxLeft  = (v->maxLeft < getSize(v) ? v->maxLeft : getSize(v));
                    }
            }
        }

        // right corner ---------------------------------------------
        {
            DATA lastkey = -INF;
            v->maxRight  = 0;
            bool isok    = true;

            if (rightExist)
            {
                v->maxRight = v->right->maxRight;
                lastkey = v->right->leftKey;

                if (v->right->maxRight < getSize(v->right))
                    isok = false;
            }

            if (isok && lastkey <= v->key)
            {
                lastkey = v->key;
                v->maxRight++;

                if (leftExist)
                    if (lastkey <= v->left->rightKey)
                    {
                        v->maxRight += v->left->maxRight;
                        v->maxRight  = (v->maxRight < getSize(v) ? v->maxRight : getSize(v));
                    }
            }
        }
    }
}

void SplayNode::ejectSegment(SplayNode *v, size_t from, size_t to, SplayNode *&prePart, SplayNode *&segment, SplayNode *&postPart)
{
    split(v, from, prePart, segment);

    if (to - from + 1 < getSize(segment))
        split(segment, to - from + 1, segment, postPart);
}

SplayNode *SplayNode::findPrevInDecreaseSegment(SplayNode *v)
{
    if (v->left != nullptr)
        return findMinInSubtree(v->left);

    SplayNode *y = v->parent;
    while (y != nullptr && v == y->left)
    {
        v = y;
        y = y->parent;
    }
    return y;
}

SplayNode *SplayNode::findMinInSubtree(SplayNode *v)
{
    if (v->right == nullptr)
         return v;
    return findMinInSubtree(v->right);
}

SplayNode *SplayNode::findKeyInDecreaseSegment(SplayNode *v, DATA key)
{
    while (v != nullptr)
    {
        pushAddRev(v);

        if (key < v->key)
        {
            if (v->right != nullptr)
                v = v->right;
            else // imaginary adding vertice
            {
                SplayNode *rightclear = v;

                SplayNode node1(key, nullptr, nullptr, v);
                v->right = &node1;

                v = SplayNode::findPrevInDecreaseSegment(&node1);
                rightclear->right = nullptr; // delete vertice
                return v->splay();
            }
        }
        else
        {
            if (v->left != nullptr)
               v = v->left;
            else // imaginary adding vertices
            {
                SplayNode *leftclear = v;

                SplayNode node1(key, nullptr, nullptr, v);
                v->left = &node1;

                v = SplayNode::findPrevInDecreaseSegment(&node1);
                leftclear->left = nullptr; // delete vertive
                return v->splay();
            }
        }
    }
}

DATA SplayNode::getKey()
{
    return this->key;
}

SplayNode * SplayNode::findByPos(SplayNode *v, size_t pos)
{
    pushAddRev(v);

    if (v == nullptr)
        return nullptr;

    size_t leftSize = getSize(v->left);
    if (pos == leftSize)
        return v->splay();

    if (pos < leftSize && v->left != nullptr)
        return findByPos(v->left, pos);
    if (pos > leftSize && v->right != nullptr)
        return findByPos(v->right, pos - leftSize - 1);

    return v->splay();
}

SplayNode* SplayNode::insert(SplayNode *&v, size_t pos, DATA key)
{
    pushAddRev(v);

    SplayNode *left, *right;
    split(v, pos, left, right);

    v = new SplayNode(key, left, right);

    keepParent(v);
    update(v);

    return v;
}

SplayNode *SplayNode::remove(SplayNode *&v, size_t pos)
{
    v = findByPos(v, pos);

    SplayNode * leftChild  = v->left;
    SplayNode * rightChild = v->right;

    setParent(leftChild,  nullptr);
    setParent(rightChild, nullptr);

    v->left = v->right = nullptr;
    delete v;

    return merge(leftChild, rightChild);
}

DATA SplayNode::reqSum(SplayNode *&v, size_t from, size_t to)
{
    SplayNode *pre  = nullptr, // [0; from)
              *seg  = nullptr, // [from, to]
              *post = nullptr; // (to; size]

    ejectSegment(v, from, to, pre, seg, post);

    DATA ans = getSum(seg);

    v = merge(pre, seg);
    v = merge(v,  post);

    return ans;
}

void SplayNode::addRange(SplayNode *&v, size_t from, size_t to, DATA addition)
{
    SplayNode *pre  = nullptr, // [0; from)
              *seg  = nullptr, // [from, to]
              *post = nullptr; // (to; size]

    ejectSegment(v, from, to, pre, seg, post);

    useAdd(seg, addition);
    pushAddRev(seg);

    v = merge(pre, seg);
    v = merge(v,  post);
}

void SplayNode::revRange(SplayNode *&v, size_t from, size_t to)
{
    SplayNode *pre  = nullptr, // [0; from)
              *seg  = nullptr, // [from, to]
              *post = nullptr; // (to; size]

    ejectSegment(v, from, to, pre, seg, post);

    useRev(seg);
    pushAddRev(seg);

    v = merge(pre, seg);
    v = merge(v,  post);
}

void SplayNode::nextPermutation(SplayNode *&v, size_t from, size_t to)
{
    SplayNode *pre = nullptr,
              *seg = nullptr,
              *post = nullptr;

    ejectSegment(v, from, to, pre, seg, post);

    // seg - our range --------------------------------------
    SplayNode *notSortedPart;
    split(seg, getSize(seg) - seg->maxRight, notSortedPart, seg);

    SplayNode *toSwapNode = nullptr; // last node in NotSortedPart

    if (getSize(notSortedPart) > 0) // or we get last permutation like "4 3 2 1"
    {
        split(notSortedPart, getSize(notSortedPart) - 1, notSortedPart, toSwapNode); // ejecting toSwapNode

        // after this operation, the tree will be suspended for the second vertex
        seg = findKeyInDecreaseSegment(seg, toSwapNode->key);

        // swapping (i) and (j) elements
        SplayNode *tt1 = nullptr,
                  *tt2 = nullptr;


        split(seg, getSize(seg->left), tt1,  seg);
        split(seg, 1,                 seg,  tt2);

        swap(toSwapNode, seg);

        seg = merge(tt1, seg);
        seg = merge(seg, tt2);
    }

    useRev(seg);
    pushAddRev(seg);

    notSortedPart = merge(notSortedPart, toSwapNode);

    seg = merge(notSortedPart, seg);

    v = merge(pre, seg);
    v = merge(v , post);
}

void SplayNode::printTree(SplayNode *v, int level)
{
    cout << "    ";
    for (int i = 0; i < level; ++i)
        cout << ". ";

    if (v == nullptr)
    {
        cout << "#" << endl;
        return;
    }
    else
    {
        cout << "[""" << v->key << """; |"<<getSize(v)<<"|; +" << v->add<<"; sum=" <<getSum(v)
             << ";   maxL=" << v->maxLeft<<"; maxR="<<v->maxRight<<"; lKey="<<v->leftKey
             << "; rKey=" << v->rightKey << "]" << endl;
        printTree(v->left, level+1);
        printTree(v->right, level+1);
    }
}

void SplayNode::printArray(SplayNode *v)
{
    if (v != nullptr)
    {
        printArray(v->left);
        cout << v->key << " ";
        printArray(v->right);
        update (v);
    }
}

// SPLAY TREE -----------------------------------------------------------------------------

SplayTree::SplayTree()
{
    head = nullptr;
}

SplayTree::~SplayTree()
{
    delete head;
}

void SplayTree::insert(size_t pos, DATA key)
{
    head = SplayNode::insert(head, pos, key);
}

void SplayTree::remove(size_t pos)
{
    head = SplayNode::remove(head, pos);
}

DATA SplayTree::getElem(size_t pos)
{
    head = SplayNode::findByPos(head, pos);
    return (head->getKey());
}

DATA SplayTree::sum(size_t from, size_t to)
{
    return SplayNode::reqSum(head, from, to);
}

void SplayTree::change(size_t pos, DATA newkey)
{
    head = SplayNode::remove(head, pos);
    head = SplayNode::insert(head, pos, newkey);
}

void SplayTree::add(size_t from, size_t to, DATA addition)
{
    SplayNode::addRange(head, from, to, addition);
}

void SplayTree::reverse(size_t from, size_t to)
{
    SplayNode::revRange(head, from, to);
}

void SplayTree::nextPermutation(size_t from, size_t to)
{
    SplayNode::nextPermutation(head, from, to);
}

void SplayTree::printAsTree()
{
    cout << "-----------" << endl;
    SplayNode::printTree(head, 0);
    cout << endl;
}

void SplayTree::print()
{
    cout << "[";
    SplayNode::printArray(head);
    cout << "]" << endl;
}
