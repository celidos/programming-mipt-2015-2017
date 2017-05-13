#include <avltree.h>

CAVLtree::CAVLtree()
{
    head = NULL;
}

CAVLtree::~CAVLtree()
{

}

void CAVLtree::printTree()
{
    cout << "Tree: ";
    printNode(head);
    cout << endl;
}

int CAVLtree::getHeight(SNode *tree)
{
    if (tree != NULL)
        return tree->h;
    return 0;
}

int CAVLtree::getBalance(SNode *tree)
{
    return getHeight(tree->right) - getHeight(tree->left);
}

int CAVLtree::getTreeSize(SNode *tree)
{
    if (tree != NULL)
        return tree->treeSize;
    return 0;
}

// восстанавливает высоту вершины
void CAVLtree::refreshHeight(SNode *tree)
{
    int leftH  = getHeight(tree->left);
    int rightH = getHeight(tree->right);

    if (leftH > rightH)
        tree->h = leftH + 1;
    else
        tree->h = rightH + 1;
}

void CAVLtree::refreshTreeSize(SNode *tree)
{
    if (tree != NULL)
        tree->treeSize = 1 + getTreeSize(tree->left) + getTreeSize(tree->right);
}

// правый поворот вокруг вершины tree
SNode * CAVLtree::rotateRight(SNode *tree)
{
    SNode* lChild = tree->left;

    tree->left = lChild->right;
    lChild->right = tree;

    refreshHeight(tree);
    refreshHeight(lChild);

    refreshTreeSize(tree);
    refreshTreeSize(lChild);

    return lChild;
}

// левый поворот вокруг вершины tree
SNode * CAVLtree::rotateLeft(SNode *tree)
{
    SNode* rChild = tree->right;

    tree->right = rChild->left;
    rChild->left = tree;

    refreshHeight(tree);
    refreshHeight(rChild);

    refreshTreeSize(tree);
    refreshTreeSize(rChild);

    return rChild;
}

// восстанавливает баланс вершины tree
SNode * CAVLtree::refreshBalance(SNode *tree)
{
    refreshHeight(tree);
    refreshTreeSize(tree);

    if (getBalance(tree) == -2)
    {
        if (getBalance(tree->left) > 0)
            tree->left = rotateLeft(tree->left);

        return rotateRight(tree);
    }

    if (getBalance(tree) == 2)
    {
        if (getBalance(tree->right) < 0)
            tree->right = rotateRight(tree->right);

        return rotateLeft(tree);
    }

    return tree;
}

SNode * CAVLtree::insert(SNode *tree, DATA key)
{
    if (tree == NULL)
        return new SNode(key);

    if (key < tree->key)
        tree->left  = insert(tree->left,  key);
    else
        tree->right = insert(tree->right, key);

    tree = refreshBalance(tree);
    refreshTreeSize(tree);
    return tree;
}

// ищет узел с минимальным ключом в дереве tree
SNode * CAVLtree::searchMinNode(SNode *tree)
{
    if (tree->left != NULL)
        return searchMinNode(tree->left);

    return tree;
}

SNode *CAVLtree::removeMin(SNode *tree)
{
    if (tree->left == NULL)
        return tree->right;

    tree->left = removeMin(tree->left);

    tree = refreshBalance(tree);
    refreshTreeSize(tree);
    return tree;
}

SNode *CAVLtree::remove(SNode *tree, DATA key)
{
    if (tree == NULL)
        return NULL;

    if (key < tree->key)
        tree->left  = remove(tree->left, key);
    else if (key > tree->key)
        tree->right = remove(tree->right, key);
    else
    {
        SNode* lChild = tree->left;
        SNode* rChild = tree->right;

        tree->left = tree->right = NULL;
        delete tree;

        if (rChild == NULL)
            return lChild;

        SNode* min = searchMinNode(rChild);

        min->right = removeMin(rChild);
        min->left  = lChild;

        min = refreshBalance(min);
        refreshTreeSize(min);
        return min;
    }
    tree = refreshBalance(tree);
    refreshTreeSize(tree);
    return tree;
}

DATA CAVLtree::getKstat(SNode *tree, int k)
{
    if (tree == NULL)
        return 15005050;


    int lsize = getTreeSize(tree->left);
    if (k <= lsize)
        return getKstat(tree->left, k);
    else if (lsize == k - 1)
        return tree->key;
    else
        return getKstat(tree->right, k - lsize - 1);
}

void CAVLtree::printNode(SNode *tree)
{
    if (tree == NULL)
        cout << "# ";
    else
    {
        cout << tree->key <<"[" <<getTreeSize(tree) << "] L(";
        printNode(tree->left);
        cout << ") R(";
        printNode(tree->right);
        cout << ") ";
    }
}

