#include <iostream>

#include "myparallel_sort.h"

using namespace std;

int nn[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 50, 100, 500, 10000, 50000, 400000, 1000000};

int main()
{


    cout << "Tester -------------------------------" << endl;

    for (int Threads = 1; Threads < 10; ++Threads)
    {
        for (int nIndex = 0; nIndex < 19; ++nIndex)
        {
            for (int attempt = 0; attempt < 5; ++attempt)
            {

                int n = nn[nIndex];
                cout << "THREADS : " << Threads << "; ELEMENTS : " << n << "; attempt #" << attempt << " ...";
                parallel_sort psort(Threads);
                vector<int> v1, v2;

                for (int i = 0; i < n; ++i)
                {
                    //t = (t*113+7)%203;
                    //v1.push_back(t);
                    v1.push_back(i);
                } std::random_shuffle(v1.begin(), v1.end());

                for (int i = 0; i < n; ++i)
                {
                    //t = (t*113+7)%203;
                    //v1.push_back(t);
                    v2.push_back(v1[i]);
                }

                /*cout << "PRINTING ARRAY" << endl;
                for (int i = 0; i < n; ++i)
                {
                    cout << v1[i] << " ";
                }
                cout << endl;*/

                //cout << "usual sort v1 : " << endl;

                sort(v1.begin(), v1.end());

                /*for (int i = 0; i < n; ++i)
                {
                    cout << v1[i] << " ";
                }
                cout << endl;*/

                //cout << "my fucking sort v2 : " << endl;

                psort.sort(v2.begin(), v2.end(), std::less<int>());
                cout << "done! Checking... ";

                for (int i = 0; i < n; ++i)
                {
                    //cout << v2[i] << " ";
                    if (v1[i] != v2[i])
                    {
                        cout << "ERROR" << endl;
                        return -1;
                    }
                }
                cout << " ok!" << endl;
            }
        }
    }

    return 0;
}
