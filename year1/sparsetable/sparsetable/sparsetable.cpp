#include <sparsetable.h>

CSparseTable::CSparseTable()
{
}

CSparseTable::CSparseTable(vector<DATA> &array)
{
    destroyTable();

    buildOn(array);
}

CSparseTable::~CSparseTable()
{
    destroyTable();
}

void CSparseTable::buildOn(vector<DATA> &array)
{
    destroyTable();

    arrsz = array.size();

    table.push_back(new SPair[arrsz]);
    for (int j = 0; j < arrsz; ++j)
        table[0][j] = SPair(SElement(array[j], j), SElement(INF));

    for (int i = 1; (1 << i) < arrsz; ++i)
    {
        int len = (1 << i);
        table.push_back(new SPair [arrsz - len + 1]);

        for (int j = 0; j < arrsz - len + 1; ++j)
            refreshTableElement(i, j, len);

    }
}

DATA CSparseTable::getSecondStat(int left, int right)
{
    if (left > right)   swap(left, right);
    if (left < 0)       left = 0;
    if (right >= arrsz) right = arrsz - 1;

    int len = right - left + 1;
    int k = findNearPowerOf2(len);
    int secindex = right - (1 << k) + 1;

    SElement max1;
    if (table[k][left].stat1.key < table[k][secindex].stat1.key)
        max1 = table[k][left].stat1;
    else
        max1 = table[k][secindex].stat1;

    // Вторая статистика
    SElement stat2(INF);
    if (table[k][left].stat1     < stat2 && table[k][left].stat1     != max1) stat2 = table[k][left].stat1;
    if (table[k][left].stat2     < stat2 && table[k][left].stat2     != max1) stat2 = table[k][left].stat2;
    if (table[k][secindex].stat1 < stat2 && table[k][secindex].stat1 != max1) stat2 = table[k][secindex].stat1;
    if (table[k][secindex].stat2 < stat2 && table[k][secindex].stat2 != max1) stat2 = table[k][secindex].stat2;

    return stat2.key;
}

void CSparseTable::destroyTable()
{
    arrsz = 0;
    for (int i = 0; i < (int)table.size(); ++i)
    {
        if (table[i] != NULL)
            delete [] table[i];
    }

    table.clear();
}

int CSparseTable::findNearPowerOf2(DATA x)
{
    int ans = 0;
    for (ans = 0; (1 << ans) <= x; ++ans);
    return ans - 1;
}

void CSparseTable::refreshTableElement(int i, int j, int len)
{
    // Первая статистика
    if (table[i - 1][j].stat1.key < table[i - 1][j + len/2].stat1.key)
        table[i][j].stat1 = table[i - 1][j].stat1;
    else
        table[i][j].stat1 = table[i - 1][j + len/2].stat1;

    // Вторая статистика
    SElement newstat2(INF);
    if (table[i - 1][j].stat1         < newstat2 && table[i - 1][j].stat1         != table[i][j].stat1) newstat2 = table[i - 1][j].stat1;
    if (table[i - 1][j].stat2         < newstat2 && table[i - 1][j].stat2         != table[i][j].stat1) newstat2 = table[i - 1][j].stat2;
    if (table[i - 1][j + len/2].stat1 < newstat2 && table[i - 1][j + len/2].stat1 != table[i][j].stat1) newstat2 = table[i - 1][j + len/2].stat1;
    if (table[i - 1][j + len/2].stat2 < newstat2 && table[i - 1][j + len/2].stat2 != table[i][j].stat1) newstat2 = table[i - 1][j + len/2].stat2;

    table[i][j].stat2 = newstat2;
}
