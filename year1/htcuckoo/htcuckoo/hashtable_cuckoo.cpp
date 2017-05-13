#include <hashtable_cuckoo.h>

CHashTable::CHashTable() :
    countElem(0), maxSize(HT_START_TABLE_SIZE)
{
    initRandom();

    htable = vector <string> (HT_START_TABLE_SIZE);

    chooseHashFunctions();
}

bool CHashTable::add(const std::string &key)
{
    if (!has(key))
    {
        ++addingIteration;

        hash hs1 = hf1.gethash(key, maxSize),
             hs2 = hf2.gethash(key, maxSize);

        int counter = 0;
        string memory = key;
        int switcher = 1;
        while (counter < maxSize)
        {
            if (htable[hs1].empty())
            {
                htable[hs1] = memory;
                ++countElem;
            }
            else if (htable[hs2].empty())
            {
                htable[hs2] = key;
                ++countElem;
            }

            if (switcher)
            {
                memory = htable[hs1];
                htable[hs1] = key;
                if (wasthere[hs1] == addingIteration)
                {
                    rebuildTable();
                    add(key);
                    return true;
                }

                hs2 = hf2.gethash(memory);
            }
            else
            {
                memory = htable[hs2];
                htable[hs2] = key;
                if (wasthere[hs1] == addingIteration)
                {
                    rebuildTable();
                    add(key);
                    return true;
                }


                hs1 = hf1.gethash(memory);
            }

            ++counter;
            switcher = 1 - switcher;
        }
    }
}

void CHashTable::initRandom() const
{
    srand(time(0));
}

void CHashTable::chooseHashFunctions()
{
    hf1.rechoose();
    hf2.rechoose();

    while (!hf1.equiv(hf2))
        hf2.rechoose();
}

CHashFunction::CHashFunction()
{
    rechoose();
}

hash CHashFunction::gethash(std::string str, int M) const
{
    hash hs = 0;
    for (int i = 0; i < (int)str.length(); ++i)
        hs = (hs * _hashA + str[i]) % HT_PRIME_MOD1;

    hs %= M;
    return hs;
}

void CHashFunction::rechoose()
{
    _hashA = rand() % (HT_PRIME_MOD1 - 1) + 1;
}

bool CHashFunction::equiv(CHashFunction &hf) const
{
    return (this->_hashA == hf.hashA());
}


