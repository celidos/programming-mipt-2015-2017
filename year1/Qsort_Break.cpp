#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::swap;
using std::vector;

void qsort(int *a, int l, int r)
{
    if (l == r)
        return;

    cout << "call qsort with (l = " << l + 1 << "; r = " << r + 1 << ")  ";
    if (l >= r)
        return;
    int x = a[(l + r) / 2];
    cout << "  x = " << x << endl;
    int i = l;
    int j = r;
    while( i <= j )
    {
        while( a[i] < x )
            ++i;
        while( a[j] > x )
            --j;
        if( i <= j )
        {
            swap(a[i], a[j]);
            ++i;
            --j;
        }
    }
    qsort(a, l, j);
    qsort(a, i, r);
}

struct mypair
{
    int index, value;
};

void GenerateKillerSequence(int *a, int N)
{
    mypair *M = new mypair[N];

    for (int i = 0; i < N; ++i)
        M[i].index = i + 1;

    int l = 0, r = N - 1;
    int newvalue = 1;
    while (l <= r)
    {
        int m = (l + r) / 2;
        M[m].value = newvalue++;
        swap(M[m], M[l]);
        ++l;
    }

    for (int i = 0; i < N; ++i)
        a[M[i].index - 1] = M[i].value;

    cout << endl;



    delete [] M;
}

int main()
{
    int N;
    cin >> N;

    int *a = new int [N];

    GenerateKillerSequence(a, N);

    cout << " Generated : " << endl;
    for (int i = 0; i < N; ++i)
        cout << a[i] << " ";
    cout << endl << "QSort starting..." << endl;

    qsort(a, 0, N - 1);

    cout << " After sort : " << endl;
    for (int i = 0; i < N; ++i)
        cout << a[i] << " ";
    cout << endl;

    delete [] a;

    return 0;
}

