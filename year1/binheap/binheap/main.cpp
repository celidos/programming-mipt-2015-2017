#include <iostream>
#include <binheaps.h>

using namespace std;

int main()
{
    CHeapsManager bheaps;

    string req;
    int k, m;

    while (cin >> req >> k)
    {
        if (req == "add")
            bheaps.addHeap(k);
        else if (req == "insert")
        {
            cin >> m;
            bheaps.insert(k, m);
        }
        else if (req == "min")
            cout << bheaps.extractMin(k) << endl;
        else
        {
            cin >> m;
            bheaps.meld(k, m);
        }
        //bheaps._printAll();
    }

    return 0;
}

