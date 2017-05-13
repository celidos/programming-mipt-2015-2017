#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

typedef int hash;

const int HT_PRIME_MOD1       = 106033;
const int HT_PRIME_MOD2       = 115249;
const int HT_PRIME_MOD3       = 16769023;
const int HT_PRIME_MOD4       = 6679881;
const int HT_START_TABLE_SIZE = 8;

const int HT_FILL_RATIO1      = 3;
const int HT_FILL_RATIO2      = 4;

const int HT_EMPTY_RATIO1     = 1;
const int HT_EMPTY_RATIO2     = 4;
const int HT_GROW_RATIO       = 4;

const string HT_EMPTY_STR = "~\n~";

// -------------------------------------------------------

struct doublehash
{
    hash h1;
    hash h2;
    doublehash(hash H1, hash H2)
    {
        h1 = H1;
        h2 = H2;
    }
};

class CHashTable
{
public:
    CHashTable();
    ~CHashTable() {};

    int  has  (const string &key);
    bool add  (const string &key);
    bool erase(const string &key);
private:
    vector <string> htable;
    int countElem;
    int maxSize;

    doublehash getdhash (const string &str);

    void extendMemory();
    void reduceMemory();
    void resizeMemory(int newSize);
};

#endif // HASHTABLE_H

