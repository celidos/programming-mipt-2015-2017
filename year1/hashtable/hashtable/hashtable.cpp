#include <hashtable.h>

doublehash CHashTable::getdhash(const std::string &str)
{
    long long int hs1 = 0, hs2 = 0;

    for (int i = 0; i < (int)str.length(); ++i)
    {
        hs1 = (hs1 * (long long int) HT_PRIME_MOD1 + str[i]) % HT_PRIME_MOD3;
        hs2 = 1(hs2 * (long long int) HT_PRIME_MOD2 + str[i]) % HT_PRIME_MOD4;
    }

    hs2 %= (maxSize - 1);
    if (hs2 % 2 == 0)
        hs2 = (hs2 + 1);
    hs1 %= maxSize;

    doublehash res((hash) hs1, (hash) hs2);
    return res;
}

void CHashTable::extendMemory()
{
    resizeMemory(maxSize * HT_GROW_RATIO);
}

void CHashTable::reduceMemory()
{
    resizeMemory(maxSize / HT_GROW_RATIO);
}

void CHashTable::resizeMemory(int newSize)
{
    maxSize = newSize;
    vector <string> newtable(maxSize);
    for (int i = 0; i < htable.size(); ++i)
    {
        if (!htable[i].empty() && htable[i] != HT_EMPTY_STR)
        {
            doublehash h = getdhash(htable[i]);

            while (!newtable[h.h1].empty())
                h.h1 = (h.h1 + h.h2) % maxSize;

            newtable[h.h1] = htable[i];
        }
    }

    htable = newtable;
}

CHashTable::CHashTable() :
    countElem(0), maxSize(HT_START_TABLE_SIZE)
{
    htable = vector <string> (HT_START_TABLE_SIZE);
}

int CHashTable::has(const string &key)
{
    doublehash h = getdhash(key);

    int counter = 0;
    while (!htable[h.h1].empty())
    {
        ++ counter;
        if (counter > maxSize + 2)
            return 0;

        if (htable[h.h1] == key)
            return h.h1 + 1;
        h.h1 = (h.h1 + h.h2) % maxSize;
    }

    return 0;
}

bool CHashTable::erase(const std::string &key)
{
    int place = has(key);
    if (place)
    {
        htable[place - 1] = HT_EMPTY_STR;
        --countElem;

        if (1ll * countElem * HT_EMPTY_RATIO2 < 1ll * maxSize * HT_EMPTY_RATIO1 && maxSize > HT_START_TABLE_SIZE)
            reduceMemory();

        return true;
    }
    return false;
}

bool CHashTable::add(const string &key)
{
    if (!has(key))
    {
        doublehash h = getdhash(key);

        while (!htable[h.h1].empty() && htable[h.h1] != HT_EMPTY_STR)
            h.h1 = (h.h1 + h.h2) % maxSize;

        htable[h.h1] = key;
        ++countElem;

        if (1ll * countElem * HT_FILL_RATIO2 > 1ll * maxSize * HT_FILL_RATIO1)
            extendMemory();

        return true;
    }
    return false;
}
