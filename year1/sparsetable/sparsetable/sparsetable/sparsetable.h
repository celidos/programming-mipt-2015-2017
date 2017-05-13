#ifndef SPARSETABLE_H
#define SPARSETABLE_H

#include <iostream>
#include <cmath>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::min;
using std::vector;
using std::swap;

typedef int DATA;

const DATA INF = 1234500000;

struct SElement
{
    DATA key;
    int  index;
    SElement(DATA _key = 0, int _index = -1) : key(_key), index(_index) {}
    bool operator <(const SElement b)
    {
        return ((this->index != b.index) ? (this->key < b.key) : false);
    }
    bool operator !=(const SElement b)
    {
        return (this->index != b.index) || (this->key != b.key);
    }
    void operator =(const SElement b)
    {
        key = b.key;
        index = b.index;
    }
};

struct SPair
{
    SElement stat1;
    SElement stat2;

    SPair(SElement _stat1, SElement _stat2) : stat1(_stat1), stat2(_stat2) {}
    SPair() : stat1(INF), stat2(INF) {}
};

class CSparseTable
{
public:
    CSparseTable();
    CSparseTable(vector <DATA> &array);
    ~CSparseTable();

    void buildOn(vector <DATA> &array);
    DATA getSecondStat(int left, int right);
private:
    void destroyTable();
    int  findNearPowerOf2(DATA x);
    void refreshTableElement(int i, int j, int len);

    int arrsz;

    vector <SPair *> table;
};

#endif // SPARSETABLE_H

