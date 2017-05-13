#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::swap;
using std::string;
using std::vector;

/*  [a; b) - полуинтервалы */

void DigitQuickSort(vector <string> &A, int l, int r, int dig)
{
    if (r - l <= 1)
        return;

    int m = (l + r) /2;
    char medium = (A[m])[dig];

    int lowbound = r, ndex = l;
    while (ndex < lowbound)
    {
        while (lowbound > ndex && A[lowbound - 1][dig] == medium)
            --lowbound;

        if (ndex < lowbound && (A[ndex])[dig] == medium)
            swap(A[ndex], A[--lowbound]);

        ++ndex;
    }

    int leftIndex = l, rightIndex = lowbound-1;
    while (leftIndex < rightIndex)
    {
        while ((A[leftIndex])[dig] < medium && leftIndex < rightIndex)
            ++leftIndex;

        while ((A[rightIndex])[dig] > medium && leftIndex < rightIndex)
            --rightIndex;

        if (leftIndex < rightIndex)
        {
            swap(A[leftIndex], A[rightIndex]);
            ++leftIndex;
            --rightIndex;
        }
    }

    for (int i = l; i < r; ++i)
        if (A[i][dig] >= medium)
        {
            rightIndex = i;
            break;
        }

    int delta = lowbound - rightIndex;
    for (int i = lowbound; i < r; ++i)
        swap(A[i], A[i - delta]);

    DigitQuickSort(A, l,          rightIndex, dig    ); // left
    DigitQuickSort(A, rightIndex, r - delta,  dig + 1); // middle
    DigitQuickSort(A, r - delta,  r,          dig    ); // right
}

int main()
{
    vector <string> A;

    //int N; cin >> N;

    string str;
    while (cin >> str)
    //for (int i = 0; i < N; ++i)
    {
      //  cin >> str;
        A.push_back(str);
    }

    DigitQuickSort(A, 0, A.size(), 0);

    for (int i = 0; i < A.size(); ++i)
        cout << A[i] << endl;

    return 0;
}

