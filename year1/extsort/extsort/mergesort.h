#ifndef MERGESORT
#define MERGESORT

template <typename T>
void _internal_merge_sort(T *A, int l, int r)
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

        T *buffer = new T[r - l];

        int leftIndex = l, rightIndex = m, index = 0;
        while (leftIndex < m && rightIndex < r)
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

            if (leftIndex == m)
            {
                for (int i = rightIndex; i < r; ++i)
                    buffer[index++] = A[i];
            }
            if (rightIndex == r)
            {
                for (int i = leftIndex; i < m; ++i)
                    buffer[index++] = A[i];
            }
        }

        for (int i = 0; i < r - l; i++)
            A[l + i] = buffer[i];

        delete [] buffer;
    };
}



#endif // MERGESORT

