#include <iostream>
#include <splaytree.h>

int main()
{
    SplayTree *head = nullptr;

    char c;
    int x;
    while (cin >> c >> x)
    {
        if (c == '+')
        {
            head = SplayTree::insert(head, x);
        }
        else if (c == '-')
        {
            head = SplayTree::remove(head, x);
        }
        else if (c == '?')
        {
            head = SplayTree::find(head, x);
        }

        cout << endl;
        SplayTree::printTree(head);
        cout << endl;
    }

    return 0;
}
