#include <skiplist.h>

#include <cmath>
using std::abs;

CSkipList::CSkipList()
{
    initRandom();

    head = NULL;
    addNewLayer();
}

CSkipList::~CSkipList()
{
    // уничтожаем список :(
    SNode *iter = head;
    while (iter != NULL)
    {
        delete iter;
        iter = iter->down;
    }
}

bool CSkipList::insert(DATA _key)
{
    return insert(head, _key);
}

bool CSkipList::erase(DATA _key)
{
    return eraseElement(head, _key);
}

bool CSkipList::search(DATA _key)
{
    return searchKey(head, _key);
}

void CSkipList::printList()
{
    SNode *iter = head;
    while (iter != NULL)
    {
        cout << "* > ";
        SNode *iter2 = iter;
        while (iter2 != NULL)
        {
            cout << iter2->key << " > ";
            iter2 = iter2->next;
        }
        iter = iter->down;
        cout << endl;
    }
}

bool CSkipList::searchKey(SNode* node, DATA &_key)
{
    if (node == NULL)
        return false;

    if (node->key == _key)
        return true;

    if (node->next->key <= _key)
        return searchKey(node->next, _key);

    return searchKey(node->down, _key);
}

bool CSkipList::insert(SNode *&node, DATA &_key)
{
    if (node == NULL)
        return false;

    if (node->key <= _key && node->next->key > _key && node->down == NULL)
    {
        if (node->key == _key)
            return false;
        else
        {
            addNodeAndPreviousLayers(node, _key);
            return true;
        }
    }

    if (node->next->key <= _key)
        return insert(node->next, _key);

    else if (node->down != NULL)
    {
        path.push(node);
        return insert(node->down, _key);
    }
}

void CSkipList::addNodeAndPreviousLayers(SNode *&node, DATA &_key)
{
    SNode* newnode = new SNode (_key);

    newnode->next = node->next;
    newnode->down = NULL;
    newnode->prev = node;

    node->next = newnode;
    newnode->next->prev = newnode;

    // -----------------------------------------

    SNode *verticalPrev = newnode;
    while (random0or1())
    {
        if (!path.empty())
        {
            SNode * el = path.top();
            path.pop();

            SNode *upnode = new SNode(_key);
            upnode->next = el->next;
            upnode->down = verticalPrev;
            upnode->prev = el;
            upnode->next->prev = upnode;
            upnode->prev->next = upnode;
            verticalPrev = upnode;
        }
        else
        {
            addNewLayer();

            SNode *upnode = new SNode(_key);
            upnode->next = head->next;
            upnode->down = verticalPrev;
            upnode->prev = head;

            upnode->next->prev = upnode;
            upnode->prev->next = upnode;

            verticalPrev = upnode;
        }
    }

    while (!path.empty())
        path.pop();
}

void CSkipList::addNewLayer()
{
    SNode *start = new SNode (MINF);
    SNode *end   = new SNode ( INF);
    start->down = head;
    start->next = end;
    start->prev = NULL;
    end->down   = NULL;
    end->next   = NULL;
    end->prev   = start;

    head = start;
}

bool CSkipList::eraseElement(SNode *&node, DATA &_key)
{
    if (node == NULL)
        return false;

    if (node->key == _key)
    {
        //cout << "enter!" << endl;
        SNode * iter = node;
        while (iter != NULL)
        {
            //cout << "delete iteration" << endl;
            iter->prev->next = iter->next;
            iter->next->prev = iter->prev;

            iter->next = iter->prev = NULL;

            SNode *step = iter->down;
            delete iter;
            iter = step;
        }
        return true;
    }


    //cout<< "don!" << endl;
    if (node->next->key <= _key)
        return eraseElement(node->next, _key);

    return eraseElement(node->down, _key);
}

int CSkipList::random0or1()
{
    return rand() % 2;
}

void CSkipList::initRandom()
{
    srand( time (0));
}
