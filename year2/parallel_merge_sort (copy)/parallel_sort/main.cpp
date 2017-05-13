#include <iostream>

#include "myparallel_sort.h"

using namespace std;

int main()
{
    parallel_sort psort(4);

    //cout << "Tester -------------------------------" << endl;

    int n = 30;

    int t = 2;
    vector<int> v1, v2;
    for (int i = 0; i < n; ++i)
    {
        t = (t*113+7)%203;
        v1.push_back(t);
        v2.push_back(t);
    }

    cout << "PRINTING ARRAY" << endl;
    for (int i = 0; i < n; ++i)
    {
        cout << v1[i] << " ";
    }
    cout << endl;

    cout << "usual sort v1 : " << endl;

    sort(v1.begin(), v1.end());

    for (int i = 0; i < n; ++i)
    {
        cout << v1[i] << " ";
    }
    cout << endl;

    cout << "my fucking sort v2 : " << endl;

    psort.sort(v2.begin(), v2.end(), std::less<int>());
    cout << "done!" << endl << "new : ";

    for (int i = 0; i < n; ++i)
    {
        cout << v2[i] << " ";
    }
    cout << endl;

    return 0;
}
