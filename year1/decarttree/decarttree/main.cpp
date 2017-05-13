#include <iostream>

#include <decarttree.h>

using namespace std;

int main()
{
    int n;
    cin >> n;

    CDecartTree dectree;
    CNaiveTree  navtree;

    int key, pr;
    for (int i = 0; i < n; ++i)
    {
        cin >> key >> pr;
        dectree.insert(key, pr);
        navtree.insert(key);
    }

    cout << "dectree.maxWidth = " << dectree.getMaxWidth() << endl;
    cout << "navtree.maxWidth = " << navtree.getMaxWidth() << endl;

    cout << abs(dectree.getMaxWidth() - navtree.getMaxWidth()) << endl;

    return 0;
}
