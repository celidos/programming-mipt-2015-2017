#ifndef HASH_H
#define HASH_H

#include <string>
#include <iostream>

using std::string;

typedef long long int lli;

class Hasher
{
private:
    static const lli p;
    lli *H;
    lli *P;
public:
    ~Hasher();

    void process(string &str);
    bool check(size_t start1, size_t finish1, size_t start2, size_t finish2);
};

void Hasher::process(string &str)
{size_t strlen = str.length();
    H = new lli[strlen + 1];
    P = new lli[strlen + 1];

    H[0] = 0;
    P[0] = 1;

    for (size_t i = 0; i < strlen; ++i)
    {
        P[i + 1] = P[i] * p;
        H[i] += (str[i] - 'a' + 1) * P[i];
        if (i)
            H[i] += H[i - 1];
    }
}

bool Hasher::check(size_t start1, size_t finish1, size_t start2, size_t finish2)
{
    lli h1  = H[finish1];
    if (start1)
        h1 -= H[start1 - 1];

    lli h2  = H[finish2];
    if (start2)
        h2 -= H[start2 - 1];

    //cout << "h1 = " <<

    // сравниваем их
    if (start1 <= start2 && h1 * P[start2 - start1] == h2 ||
        start1 > start2 && h2 * P[start1 - start2] == h1)
        return true;
    else
        return false;
}

Hasher::~Hasher()
{
    delete [] H;
    delete [] P;
}

const lli Hasher::p = 53;

#endif // HASH_H
