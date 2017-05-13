#include <splaytree.h>

SplayTree::SplayTree(DATA _key, SplayTree *_left, SplayTree *_right, SplayTree *_parent)
{
    key = _key;
    left = _left;
    right = _right;
    parent = _parent;
}

SplayTree::~SplayTree()
{
    if (left != nullptr)
        delete left;
    if (right != nullptr)
        delete right;
}

void SplayTree::keepParent(SplayTree *node)
{
    setParent(node->left,  node);
    setParent(node->right, node);
}

void SplayTree::setParent(SplayTree *child, SplayTree *newparent)
{
    if (child != nullptr)
    {
        child->parent = newparent;
    }
}

void SplayTree::rotate(SplayTree *parent, SplayTree *child)
{
    SplayTree *grandpa = parent->parent;

    if (grandpa != nullptr)
    {
        if (grandpa->left == parent)
            grandpa->left = child;
        else
            grandpa->right = child;
    }

    if (parent->left == child)
    {
        parent->left  = child->right;        // !!!!
        child ->right = parent;
    }
    else
    {
        parent->right = child->left;
        child ->left  = parent;
    }

    keepParent(child);
    keepParent(parent);

    child->parent = grandpa;
}

SplayTree * SplayTree::splay()
{
    if (this->parent == nullptr)
        return this;

    SplayTree *parent  = this->parent;
    SplayTree *grandpa = parent->parent;

    if (grandpa == nullptr)
    {
        rotate(parent, this);
        return this;
    }
    else
    {
        bool needZigZig = (grandpa->left == parent) == (parent->left == this);
        if (needZigZig)
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

SplayTree * SplayTree::find(SplayTree *v, DATA key)
{
    if (v == nullptr)
        return nullptr;

    if (key == v->key)
        return v->splay();

    if (key < v->key && v->left != nullptr)
        return find(v->left, key);
    if (key > v->key && v->right != nullptr)
        return find(v->right, key);

    return v->splay();
}

void SplayTree::split(SplayTree *&v, DATA key, SplayTree *&left, SplayTree *&right)
{
    if (v == nullptr)
    {
        left  = nullptr;
        right = nullptr;
        return;
    }

    v = find(v, key);

    if (v->key == key)
    {
        setParent(v->left, nullptr);
        setParent(v->right, nullptr);

        left  = v->left;
        right = v->right;
        return;
    }
    if (v->key < key)
    {
        right = v->right;            // !!! order !!!
        v->right = nullptr;
        setParent(right, nullptr);

        left  = v;
        //right = right;
        return;
    }
    else
    {
        left = v->left;
        v->left = nullptr;
        setParent(left, nullptr);

        // left = left;
        right = v;
        return;
    }
}

SplayTree* SplayTree::insert(SplayTree *&v, DATA key)
{
   SplayTree *left, *right;
   split(v, key, left, right);
   v = new SplayTree(key, left, right);
   keepParent(v);

   return v;
}

SplayTree *SplayTree::merge(SplayTree *left, SplayTree *right)
{
    if (right == nullptr)
        return left;
    if (left == nullptr)
        return right;

    right = find(right, left->key);

    right->left = left;                 // !!! order !!!
    left->parent = right;

    return right;
}

SplayTree *SplayTree::remove(SplayTree *&v, DATA key)
{
    v = find(v, key);

    SplayTree * leftChild = v->left;
    SplayTree * rightChild = v->right;

    setParent(leftChild, nullptr);
    setParent(rightChild, nullptr);

    v->left = v->right = nullptr;
    delete v;

    return merge(leftChild, rightChild);
}

void SplayTree::printTree(SplayTree *v)
{
    if (v == nullptr)
    {
        cout << "#";
        return;
    }
    else
    {
        cout << "[";
        cout << v->key << "; R";
        printTree(v->left);
        cout << " L";
        printTree(v->right);
        cout << "]";
    }
}


