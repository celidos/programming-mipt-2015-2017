#include <sparsetable.h>

int main()
{
    int n, m;
    cin >> n >> m;

    vector <int> v;
    int temp;
    for (int i = 0; i < n; ++i)
    {
        cin >> temp;
        v.push_back(temp);
    }

    CSparseTable sptable(v);

    int l, r;
    for (int i = 0; i < m; ++i)
    {
        cin >> l >> r;
        cout << sptable.getSecondStat(l - 1, r - 1) << endl;
    }

    return 0;
}
