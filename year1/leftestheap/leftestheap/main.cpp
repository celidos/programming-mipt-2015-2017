#include <iostream>
#include <leftestheap.h>
#include <vector>

using namespace std;

int main()
{
    CHeapsManager lheaps;

    string req;
    int k, m;

    while (cin >> req >> k)
    {
        if (req == "add")
            lheaps.addHeap(k);
        else if (req == "insert")
        {
            cin >> m;
            lheaps.insert(k, m);
        }
        else if (req == "min")
            cout << lheaps.extractMin(k) << endl;
        else
        {
            cin >> m;
            lheaps.meld(k, m);
        }

        //lheaps._printAll();
    }

    return 0;
}
