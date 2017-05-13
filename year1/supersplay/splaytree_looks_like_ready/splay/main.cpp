#include <iostream>
#include <splaytree.h>

int main()
{
    SplayTree tree;

        char c;
        int x, i, d;
        while (cin >> c >> x)
        {
            if (c == '+')
            {
                cin >> i;
                tree.insert(x - 1, i);
            }
            else if (c == '-')
            {
                tree.remove(x - 1);
            }
            else if (c == '?')
            {
                cout << tree.getElem(x - 1) << endl;
            }
            else if (c == 's')
            {
                cin >> i;
                cout << tree.getSum(x - 1, i - 1) << endl;
            }
            else if (c == 'a')
            {
                cin >> i >> d;
                tree.add(x - 1, i - 1 , d);
            }
            else if (c == 'r')
            {
                cin >> i;
                tree.reverse(x - 1, i - 1);
            }
            else if (c == 'p')
            {
                cin >> i;
                tree.nextPermutation(x - 1, i - 1);
            }
            else if (c == '/')
            {
                tree.printAsArray();
            }

            cout << endl;
            tree.print();
            cout << endl;
        }


    return 0;
}
