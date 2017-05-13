#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int * buildLcp(string str, int * suf)
{
    int N = str.length();
    int *lcp = new int[N];
    int *pos = new int[N];

    for (int i = 0; i < N; ++i)
        pos[suf[i]] = i;

    int k = 0;
    for (int i = 0; i < N; ++i)
    {
        if (k > 0)
            --k;

        if (pos[i] == N - 1)
        {
            lcp[N - 1] = -1;
            k = 0;
        }
        else
        {
            int j = suf[pos[i] + 1];
            while (std::max(i + k, j + k) < N && str[i + k] == str[j + k])
                ++k;
            lcp[pos[i]] = k;
        }
    }
    return lcp;
}

int main()
{
    int N;
    cin >> N;

    string str;
    cin >> str;

    int *suff = new int [N];

    for (int i = 0; i < N; ++i)
    {
        cin >> suff[i];
    }

    int *lcp = buildLcp("$"+ str, suff);

    for (int i = 0; i < N - 1; ++i)
    {
        cout << lcp[i] << " ";
    }
    cout << endl;

    // ----------------------------------

    delete [] suff;
    delete [] lcp;

    return 0;
}
