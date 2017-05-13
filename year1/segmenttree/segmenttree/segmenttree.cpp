#include <segmenttree.h>

CSegmentTree::CSegmentTree(int _size)
{
    dataSize = _size;
    treeSize = 4 * _size;
    data     = new int  [dataSize];
    tree     = new SNode[treeSize];
}

CSegmentTree::~CSegmentTree()
{
    delete [] data;
    delete [] tree;
}

void CSegmentTree::getDataFromKeyboard()
{
    for (int i = 0; i < dataSize; i++)
    {
        int R, G, B;
        cin >> R >> G >> B;
        data[i] = R + G + B;
    }
}

// построение дерева. Полуинтервал [left; right)
void CSegmentTree::build(int v, int left, int right)
{
    if (right - left == 1)
        tree[v] = SNode(data[left], 0);
    else
    {
        int middle = (left + right) / 2;
        build (v * 2 + 1, left,   middle);
        build (v * 2 + 2, middle, right);

        tree[v] = min(tree[v * 2 + 1].key, tree[v * 2 + 2].key);
    }
}

// получение минимума на отрезке
int CSegmentTree::getmin(int v, int left, int right, int from, int to)
{
    if ((to <= left) || (right <= from))
        return INF;
    else if ((from <= left) && (right <= to))
        return tree[v].key; // cout << "CASE !!!!" << endl; }
    else
    {
        push(v);
        int middle = (left + right) / 2;
        return min( getmin(v * 2 + 1, left, middle,  from, to),
                    getmin(v * 2 + 2, middle, right, from, to) );
    }

}

// закраска отрезка
void CSegmentTree::fill(int v, int left, int right, int from, int to, DATA with)
{
    if ((right <= from) || (to <= left))
        return;
    else if ((from <= left) && (right <= to))
    {
        tree[v].key      = with;
        tree[v].newcolor = with;
    }
    else
    {
        push(v);
        int middle = (left + right) / 2;
        fill(v * 2 + 1, left, middle,  from, to, with);
        fill(v * 2 + 2, middle, right, from, to, with);

        tree[v].key = min( tree[2 * v  + 1].key, tree[2 * v + 2].key );
    }
}

// проталкиватель. Очень нужен
void CSegmentTree::push(int v)
{
    if (tree[v].newcolor > 0)
    {
        tree[v * 2 + 1].newcolor = tree[v * 2 + 2].newcolor = tree[v].newcolor;
        tree[v * 2 + 1].key      = tree[v * 2 + 2].key      = tree[v].newcolor;
        tree[v].newcolor *= -1;
    }
}

// изменение одно элемента массива
void CSegmentTree::update(int v, int left, int right, int updateIndex, int color)
{
    if ((updateIndex < left) || (right <= updateIndex))
        return;
    else if (right - left == 1)
        tree[v] = color;
    else
    {
        push(v);
        int middle = (left + right) / 2;
        update(v * 2 + 1, left, middle,  updateIndex, color);
        update(v * 2 + 2, middle, right, updateIndex, color);

        tree[v] = min(tree[v * 2 + 1].key, tree[v * 2 + 2].key);
    }
}

