#include <decarttree.h>

CDecartTree::CNode::CNode(DATA _key, int _prior, CNode *_left, CNode *_right)
{
    key = _key;
    prior = _prior;
    left = _left;
    right = _right;
}

CDecartTree::CNode *CDecartTree::CNode::merge(CDecartTree::CNode *_left, CDecartTree::CNode *_right)
{
    CNode *l = _left;
    CNode *r = _right;

    if (l == NULL) return r;
    if (r == NULL) return l;

    if (l->prior > r->prior)
    {
        l->left = merge(l->right, r);
        return l;
    }
    else
    {
        r->right = merge(l, r->left);
        return r;
    }
}

void CDecartTree::CNode::split(DATA _key, CDecartTree::CNode *_tree, CDecartTree::CNode *&_left, CDecartTree::CNode *&_right)
{
    if (_tree == NULL)
        _left = _right = NULL;
    else if (_key < _tree->key)
    {
        split (_key, _tree->left, _left, _tree->left);
        _right = _tree;
    }
    else
    {
        split(_key, _tree->right, _tree->right, _right);
        _left = _tree;
    }
}

void CDecartTree::CNode::insert(CDecartTree::CNode *&_tree, DATA _key, int _prior)
{
    //cout << "insert called, key = " << _key  << endl;
    if (_tree == NULL)
        _tree = new CNode(_key, _prior);

    else
    {
        //cout << " else case" << endl;
        if (_prior > _tree->prior)
        {
            CNode * newnode = new CNode (_key, _prior);
            split(_key, _tree, newnode->left, newnode->right);
            _tree = newnode;
        }
        else
        {
            if (_key <= _tree->key)
                insert(_tree->left,  _key, _prior);
            else
                insert(_tree->right, _key, _prior);
        }
    }
}

void CDecartTree::CNode::erase(CDecartTree::CNode *&_tree, DATA _key)
{
    if (_tree == NULL)
        return;

    if (_tree->key == _key)
    {
        CNode *p = _tree;
        _tree = merge(_tree->left, _tree->right);

        p->left = p->right = NULL;
        delete p;
    }
    else
        if (_key < _tree->key)
            erase(_tree->left,  _key);
        else
            erase(_tree->right, _key);
}

int CDecartTree::CNode::getHeight(CDecartTree::CNode *_tree)
{
    if (_tree == NULL)
        return 0;
    return max(CNode::getHeight(_tree->left), CNode::getHeight(_tree->right)) + 1;
}

void CDecartTree::CNode::getMaxWidth(CDecartTree::CNode *_tree, int level, vector<int> &h)
{
    if (_tree == NULL) return;
    else
    {
        if (level >= (int)h.size())
            h.push_back(0);
        ++h[level];
        cout << _tree->key << ";" << _tree->prior << " ";

        cout << "L(";
        CNode::getMaxWidth(_tree->left,  level + 1, h);
        cout << ") R(";
        CNode::getMaxWidth(_tree->right, level + 1, h);
        cout << ") ";
    }
}

void CDecartTree::insert(DATA element, int priority)
{
    CNode::insert(this->head, element, priority);
}

void CDecartTree::erase(DATA key)
{
    CNode::erase(this->head, key);
}

int CDecartTree::getHeight()
{
    return CNode::getHeight(head);
}

int CDecartTree::getMaxWidth()
{
    vector <int> widths;

    widths.push_back(0);

    CNode::getMaxWidth(head, 0, widths);

    int maxWidth = 0;
    for (int i = 0; i < (int)widths.size(); ++i)
        if (widths[i] > maxWidth)
            maxWidth = widths[i];

    return maxWidth;
}

CDecartTree::CDecartTree() :
    head(NULL)
{
    //cout << "constructor called " << endl;
}

void SNaiveNode::insert(SNaiveNode * &_tree, DATA _key)
{
    if (_tree == NULL)
        _tree = new SNaiveNode(_key);
    else
    {
        if (_tree->key > _key)
            insert(_tree->l, _key);
        else
            insert(_tree->r, _key);
    }
}

int SNaiveNode::getHeight(SNaiveNode *_tree)
{
    if (_tree == NULL)
        return 0;
    return max(getHeight(_tree->l), getHeight(_tree->r)) + 1;
}

void SNaiveNode::getMaxWidth(SNaiveNode *_tree, int level, vector<int> &h)
{
    if (_tree == NULL) {cout << "# "; return;}
    else
    {
        if (level >= (int)h.size())
            h.push_back(0);

        ++h[level];
        cout << _tree->key << " ";

        cout << "L(";
        SNaiveNode::getMaxWidth(_tree->l,  level + 1, h);
        cout << ") R(";
        SNaiveNode::getMaxWidth(_tree->r,  level + 1, h);
        cout << ") ";
    }
}

int CNaiveTree::getHeight()
{
    return SNaiveNode::getHeight(head);
}

int CNaiveTree::getMaxWidth()
{
    vector <int> widths;

    widths.push_back(0);

    SNaiveNode::getMaxWidth(head, 0, widths);

    int maxWidth = 0;
    for (int i = 0; i < (int)widths.size(); ++i)
        if (widths[i] > maxWidth)
            maxWidth = widths[i];

    return maxWidth;
}
