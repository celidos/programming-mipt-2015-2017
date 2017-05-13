#include <iostream>
#include <array_tester.h>
#include <splaytree.h>
#include <naivearray.h>

int main()
{
    IArray *tree1 = new SplayTree;
    IArray *naive = new NaiveArray;

    ArrayTester tester(tree1, naive);

    tester.runTest(20000, true);

    // uncomment this to try tree in action

    /*SplayTree tree;

    char c;
    int x, i, d;
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
            cout << tree.getSum(x, i) << endl;
        }
        else if (c == 'a')
        {
            cin >> i >> d;
            tree.add(x, i , d);
        }
        else if (c == 'r')
        {
            cin >> i;
            tree.reverse(x, i);
        }
        else if (c == 'p')
        {
            cin >> i;
            tree.nextPermutation(x, i);
        }
        else if (c == '/')
        {
            tree.print();
        }

        cout << endl;
        tree.printAsTree();
        cout << endl;
    }*/

    return 0;
}
