#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int * prefix_to_zfunction(int * P, int n)
{
    int *Z = new int[n];
    for (int i = 1; i < n; ++i)
    {
        if (P[i] > 0)
            Z[i - P[i] + 1] = P[i];
    }

    Z[0] = n;
    int i = 1;
    while (i < n)
    {
        int t = i;
        if (Z[i] > 0)
        {
            for (int j = 1; j < Z[i]; ++j)
            {
                if (Z[i + j] > Z[j])
                    break;

                Z[i + j] = std::min(Z[j], Z[i] - j);
                t = i + j;
            }
        }

        i = t + 1;
    }
    return Z;
}

int main()
{
    int N;
    cin >> N;
    int * P = new int [N], * Z;

    for (int i = 0; i < N; ++i)
    {
        cin >> P[i];
    }

    Z = prefix_to_zfunction(P, N);

    for (int i = 0; i < N; ++i)
    {
        cout << Z[i] << " ";
    }
    cout << endl;

    return 0;
}
