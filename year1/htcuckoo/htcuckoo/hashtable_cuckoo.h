#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

typedef long long int hash;

//const int HT_PRIME_MOD1       = 16769023;
//const int HT_PRIME_MOD2       = 1046527;
//const int HT_PRIME_MOD3       = 106033;
const int HT_PRIME_MOD1       = 115249;

const int HT_START_TABLE_SIZE = 8;
const int HT_FILL_RATIO1      = 3;
const int HT_FILL_RATIO2      = 4;
const int HT_EMPTY_RATIO1     = 1;
const int HT_EMPTY_RATIO2     = 4;
const int HT_GROW_RATIO       = 4;

const string HT_EMPTY_STR = "~\n~";

// -------------------------------------------------------

class CHashFunction
{
public:
    CHashFunction();
    ~CHashFunction(){};

    hash gethash(const string str, int M) const;
    bool equiv(CHashFunction &hf) const;

    void rechoose();
    hash hashA() const {return _hashA;}
private:
    hash _hashA;
};

class CHashTable
{
public:
    CHashTable();
    ~CHashTable() {};

    int  has  (const string &key) const;
    bool add  (const string &key);
    bool erase(const string &key);
private:
    vector <string> htable;

    vector <int> wasthere;
    int addingIteration;

    int countElem;
    int maxSize;

    CHashFunction hf1;
    CHashFunction hf2;

    void initRandom() const;
    void chooseHashFunctions();

    void rebuildTable();
};

#endif // HASHTABLE_H
