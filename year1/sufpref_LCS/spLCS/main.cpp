#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#define NUM_OF_TESTS 200
#define MAX_STR_LEN 4
#define SYMBOL_DIFF 5

using std::vector;
using std::pair;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::max;

// --------------------- WHAT ARE YOU LOOKING FOR ------------------------------------

typedef unsigned int uint;

void transposition(vector<vector<uint> > &v)
{
    size_t N = v.size();
    size_t M = (N ? v[0].size() : 0);

    vector<vector<uint> > newv;
    for (size_t i = 0; i < M; ++i)
        newv.push_back(vector <uint> (N));

    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
            newv[j][i] = v[i][j];

    v = newv;
}

vector<vector<uint> > calculateSuffixPrefixLCS(const string &a, const string &b)
{
    // Init --------------------------

    const string &B = a;
    const string &A = b;

    vector< pair<uint, uint>* > g;   // graph

    size_t N = A.length();
    size_t M = B.length();

    for (size_t i = 0; i <= N; ++i)
        g.push_back(new pair<uint, uint> [M + 1]);

    for (size_t i = 0; i <= N; ++i)
        g[i][0].second = 0;

    for (size_t j = 0; j <= M; ++j)
        g[0][j].first = j + 1;

    // Dynamics -----------------------

    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
        {
            uint x = g[i][j].first;
            uint y = g[i][j].second;

            if (y < x && A[i] != B[j])
            {
                g[i][j+1].second = y;
                g[i+1][j].first  = x;
            }
            else
            {
                g[i][j+1].second = x;
                g[i+1][j].first  = y;
            }
        }

    // Filling table ------------------

    vector<vector<uint> > table;
    table.push_back( vector<uint>(M, 0) );

    for (size_t i = 0; i < N; ++i)
    {
        table.push_back( vector<uint>(M, -0xDEAD) );

        for (int j = M - 1; j >= 0; --j)
            table[i+1][j] = table[i][j] + 1 - static_cast<uint>(static_cast<uint>(j) >= g[i][M].second);
    }

    // Free memory --------------------

    for (auto i = g.begin(); i != g.end(); ++i)
        delete [] *i;

    table.erase(table.begin());
    transposition(table);

    return table;
}

// --------------------- TESTER -------------------------------------------

size_t LcsInefficient(const string& a, const string& b)
{
    size_t alen = a.length();
    size_t blen = b.length();

    vector<vector<int> > max_len;
    max_len.resize(alen + 1);

    for(int i = 0; i <= static_cast<int>(alen); i++)
        max_len[i].resize(b.size() + 1);

    for (int i = alen - 1; i >= 0; i--)
        for (int j = blen - 1; j >= 0; j--)
        {
            if(a[i] == b[j])
                max_len[i][j] = 1 + max_len[i+1][j+1];
            else
                max_len[i][j] = max(max_len[i+1][j], max_len[i][j+1]);
        }

    return max_len[0][0];
}

string genRandomString()
{
    string res;
    size_t len1 = abs(rand() % MAX_STR_LEN) + 1;
    for (size_t i = 0; i < len1; ++i)
        res += 'a' + abs(rand() % SYMBOL_DIFF);

    return res;
}

void testLCS()
{
    srand(time (NULL));

    for (int t = 0; t < NUM_OF_TESTS; ++t)
    {
        cout << "Test #" << t << ": " << endl;

        string str1 = genRandomString();
        string str2 = genRandomString();

        cout << "string1 = " << str1 << endl <<
                "string2 = " << str2 << endl;

        vector<vector<uint> > table = calculateSuffixPrefixLCS(str1, str2);

        // Checking
        for (size_t i = 0; i < table.size(); ++i)
            for (size_t j = 0; j < table[i].size(); ++j)
            {
                size_t expected = LcsInefficient(str1.substr(i),
                                                 str2.substr(0, j+1));

                if (table[i][j] != expected)
                {
                    cout << "Error: LCS[" << i << "][" << j << "] is incorrect (" << table[i][j]
                         << " instead of " << expected << ")" << endl;

                    cout << "STRINGS are : " << str1.substr(i) << endl
                         << "              " << str2.substr(0, j+1) << endl;

                    return;
                }
            }

        cout << "...OK!" << endl << endl;
    }
}

int main()
{
    /*string str1, str2;
    cin >> str1 >> str2;
    ... = calculateSuffixPrefixLCS(str1, str2);*/

    testLCS();

    return 0;
}
