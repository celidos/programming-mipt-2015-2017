#include <iostream>

#include <hashtable.h>

int main()
{
    string req;
    char op;

    CHashTable ht;

    while (cin >> op >> req)
    {
        bool flag = true;

        if      (op == '+')
            flag = ht.add  (req);
        else if (op == '-')
            flag = ht.erase(req);
        else
            flag = ht.has  (req);

        cout << (flag ? "OK" : "FAIL") << endl;
    }

    return 0;
}
