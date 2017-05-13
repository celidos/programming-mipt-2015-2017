#ifndef HASH_H
#define HASH_H

#include <string>
#include <cstdio>
#include <iostream>
#include <cstring>

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

    void process(char *str);
    bool check(size_t start1, size_t finish1, size_t start2, size_t finish2);
};

void Hasher::process(char *str)
{
    std::ios::sync_with_stdio(false);

    size_t strln = strlen(str);
    H = new lli[strln + 1];
    P = new lli[strln + 1];

    H[0] = 0;
    P[0] = 1;

    for (size_t i = 0; i < strln; ++i)
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
    //delete [] H;
    //delete [] P;
}

const lli Hasher::p = 53;

#endif // HASH_H

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main()
{
    char str[100005];
    scanf("%s", str);

    Hasher hs;
    hs.process(str);

    int M;
    scanf("%d", &M);
    for (int tt = 0; tt < M; ++tt)
    {
        size_t a, b, c, d;
        scanf("%d %d %d %d", &a, &b, &c, &d);//cin >> a >> b >> c >> d;

        if (hs.check(a - 1, b - 1, c - 1, d - 1))
            printf("Yes\n");
        else
            printf("No\n");
    }

    return 0;
}
