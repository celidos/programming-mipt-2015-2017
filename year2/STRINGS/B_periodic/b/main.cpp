#include <string>
#include <iostream>
#include <algorithm>

#define INF 100000000

using std::cin;
using std::cout;
using std::endl;
using std::string;

int * z_function(string &s)
{
    int n = (int)s.length();
    int * z = new int [n];
    z[0] = 0;
    for (int i = 1, l = 0, r = 0; i < n; ++i)
    {
        if (i <= r)
            z[i] = std::min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];
        if (i + z[i] - 1 > r)
        {
            l = i;
            r = i + z[i]-1;
        }
    }
    return z;
}

int * p_function(string &s)
{
    int n = (int) s.length();

    int * pi = new int[n];
    pi[0] = 0;
    for (int i = 1; i < n; ++i)
    {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j])
            j = pi[j-1];

        if (s[i] == s[j])
            ++j;

        pi[i] = j;
    }
    return pi;
}

int main()
{
    string str;
    cin >> str;

    if (str[0] == '1')
    {
        cout << "1.(0)"<< endl;
        return 0;
    }
    else
    {
        str = str.substr(2);

        int n = str.length();

        std::reverse(str.begin(), str.end());

        //int *Z = z_function(str);
        int *P = p_function(str);

        int best_j = n;
        //int betLen = INF;

        // ---------------------------

        int pr = 0, bestPeriod = n;
        for (int i = n - 1; i >= 0; --i)
        {
            int k = i - P[i] + 1;
            int curPeriod = k;
            int curPr = n - i - 1;

            if (curPeriod + curPr <= pr + bestPeriod)
            {
                bestPeriod = curPeriod;
                pr = curPr;
            }
        }

        reverse(str.begin(), str.end());

        cout << "0.";
        for (int i = 0; i < pr; i++)
           cout << str[i];

        cout << "(";
        for (int i = pr; i < pr + bestPeriod; i++)
            cout << str[i];
        cout << ")" << endl;
    }

    return 0;
}
