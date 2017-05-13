#include <skiplist.h>

int main()
{
    CSkipList slist;

    string t;
    char op;
    while (cin >> op >> t)
    {
        bool result = true;

        if (op == '+')
            result = slist.insert(t);
        else if (op == '-')
            result = slist.erase(t);
        else
            result = slist.search(t);

        cout << (result ? "OK" : "FAIL") << endl;

        slist.printList();
    }

    return 0;
}
