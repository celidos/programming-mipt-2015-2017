#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::swap;
using std::vector;

template <typename T>
void Merge(vector <T> &A, int from1, int to1, int from2, int to2, int where)
{
    vector <T> buffer;
    int leftIndex = from1, rightIndex = from2;
    while (leftIndex < to1 && rightIndex < to2)
    {
        if (A[leftIndex] > A[rightIndex])
        {
            buffer.push_back(A[rightIndex]);
            ++rightIndex;
        }
        else
        {
            buffer.push_back(A[leftIndex]);
            ++leftIndex;
        }

        if (leftIndex == to1)
        {
            for (int i = rightIndex; i < to2; ++i)
                buffer.push_back(A[i]);
        }
        if (rightIndex == to2)
        {
            for (int i = leftIndex; i < to1; ++i)
                buffer.push_back(A[i]);
        }
    }

    for (int i = 0; i < buffer.size(); ++i)
        A[i + where] = buffer[i];

}

template <typename T>
void _internal_merge_sort(vector <T> &A, int l, int r)
{
    if (r - l == 2)
    {
        if (A[l] > A[l+1])
            swap(A[l], A[l+1]);
    }
    else if (r - l > 2)
    {
        int m = (l + r) / 2;

        _internal_merge_sort(A, l, m);
        _internal_merge_sort(A, m, r);

        Merge(A, l, m, m, r, l);
    }
}

template <typename T>
void MergeSort(vector <T> &Array, int from, int to)
{
    _internal_merge_sort(Array, from, to);
}

template <typename T>
void HalfMerge(vector <T> &A, int from, int k)
{
    vector <T> buffer(k);

    int t = from + 2 * k, asize = A.size();
    int tt = (t < asize ? t : asize);
    int leftIndex = from, rightIndex = from + k, index = 0;

    if (rightIndex > asize)
        return;

    while (leftIndex < from + k &&
           rightIndex < tt &&
           index < k)
    {
        if (A[leftIndex] > A[rightIndex])
        {
            buffer[index] = A[rightIndex];
            ++rightIndex;
        }
        else
        {
            buffer[index] = A[leftIndex];
            ++leftIndex;
        }
        ++index;

        if (leftIndex == from + k)
            for (int i = rightIndex; i < tt && index < k; ++i)
                buffer[index++] = A[i];
        
        if (rightIndex == tt)
            for (int i = leftIndex; i < from + k && index < k; ++i)
                buffer[index++] = A[i];
    }

    Merge(A, leftIndex, from + k, rightIndex, tt, from+k);

    for (int i = 0; i < k; ++i)
        A[from + i] = buffer[i];
}

int main()
{
    vector <int> A;
    
    int N, k;
    cin >> N >> k;

    int temp;
    for (int i = 0; i < N; ++i)
    {
        cin >> temp;
        A.push_back(temp);
    }

    for (int i = 0; i < N; i += k)
        MergeSort(A, i, (i+k<N?i+k:N));

    for (int i = 0; i < N-k; i += k)
        HalfMerge(A, i, k);

    for (int i = 0; i < A.size(); ++i)
        cout << A[i] << " ";
    cout << endl;

    return 0;
}

