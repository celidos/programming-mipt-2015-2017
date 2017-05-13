#include <iostream>
#include <string>

using namespace std;

class MasterOfLCS
{
private:
    string S;
    string T;
    int Slen;
    int Tlen;

    int ***L;                               // T tables which are ~ (S x T) size

    int ** buildfromT(int fromTindex);
    void   freearrayL();

public:
    MasterOfLCS() { L = NULL; }
    ~MasterOfLCS() { freearrayL(); }

    void operator= (const MasterOfLCS&) = delete;
    MasterOfLCS    (const MasterOfLCS&) = delete;

    void getStrings();
    void process();
    int  reqest(int from, int to);
};

void MasterOfLCS::getStrings()
{
    std::cin >> S >> T;
    Slen = S.length();
    Tlen = T.length();
}

int ** MasterOfLCS::buildfromT(int fromTindex)
{
    int **M = new int* [Slen];
    int partlen = Tlen - fromTindex + 1;

    for (int j = 0; j < Slen; ++j)
    {
        M[j] = new int [partlen];

        for (int k = 0; k < partlen; ++k)
        {
            if (S[j] == T[fromTindex + k])
                M[j][k] = ((j > 0 && k > 0) ? M[j-1][k-1] : 0) + 1;
            else
            {
                int maxlen = 0;
                if (k > 0) maxlen = max(maxlen, M[j  ][k-1]);
                if (j > 0) maxlen = max(maxlen, M[j-1][k  ]);
                M[j][k] = maxlen;
            }
        }
    }
    return M;
}

void MasterOfLCS::freearrayL()
{
    if (L != NULL)
    {
        for (int i = 0; i < Tlen; ++i)
        {
            for (int j = 0; j < Slen; ++j)
                delete [] L[i][j];

            delete [] L[i];
        }

        delete [] L;
    }
}

void MasterOfLCS::process()
{
    freearrayL();

    L = new int** [Tlen];
    for (int i = 0; i < Tlen; ++i)
        L[i] = buildfromT(i);
}

int MasterOfLCS::reqest(int from, int to)
{
    return L[from][Slen - 1][to - from];
}

int main()
{
    MasterOfLCS master;

    master.getStrings();
    master.process();

    int l, r;
    while (cin >> l >> r)
        cout << master.reqest(l - 1, r - 1) << endl;

    return 0;
}
