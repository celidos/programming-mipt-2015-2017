#include <avltree.h>

int main()
{
    int n;
    cin >> n;

    CAVLtree tree;

    int key, k;
    for (int i = 0; i < n; ++i)
    {
        cin >> key >> k;
        if (key > 0)
            tree.insert(abs(key));
        else
            tree.erase (abs(key));

        //tree.printTree();
        cout << tree.getKstat(k) << endl;
    }
    return 0;
}
