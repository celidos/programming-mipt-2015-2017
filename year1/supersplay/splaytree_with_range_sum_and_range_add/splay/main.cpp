#include <iostream>
#include <splaytree.h>

int main()
{
    SplayTree tree;

        char c;
        int x, i;
        while (cin >> c >> x)
        {
            if (c == '+')
            {
                cin >> i;
                tree.insert(x, i);
            }
            else if (c == '-')
            {
                tree.remove(x);
            }
            else if (c == '?')
            {
                cout << tree.getElem(x) << endl;
            }
            else if (c == 's')
            {
                cin >> i;
                cout << tree.getSum(x-1, i-1) << endl;
            }

            cout << endl;
            tree.print();
            cout << endl;
        }


    return 0;
}
