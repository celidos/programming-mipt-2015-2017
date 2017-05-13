#include <string>
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <cstdlib>

#define INF 100000000

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::set;
using std::vector;

const size_t LETTERS_IN_THE_ALPHABET = 27;

class bor
{
private:
    class vertex
    {
    public:
        vertex()
        {
            leaf = false;
            child.resize(LETTERS_IN_THE_ALPHABET, nullptr);
            size = 0;
        }
        bool leaf;
        int size;

        vector<vertex*> child;
    };

    vertex* root;

    int getVertexSize(vertex *v)
    {
        if (!v)
            return 0;

        int size = 1;
        for (size_t i = 0; i < v->child.size(); ++i)
            size += getVertexSize(v->child[i]);
        return size;
    }
    void add(vertex* v, const string &str, int pos)
    {
        if (pos == str.size())
        {
            v->leaf = true;
            v->size = 1;
            return;
        }

        if (v->child[str[pos] - 'a'] != nullptr)
        {
            add(v->child[str[pos] - 'a'], str, pos + 1);
            v->size++;
        }
        else
        {
            v->child[str[pos] - 'a'] = new vertex();
            add(v->child[str[pos] - 'a'], str, pos + 1);
            v->size++;
        }
    }

public:

    bor()
    {
        root = new vertex();
    }

    void add(string s)
    {
        add(root, s, 0);
    }

    int getSize()
    {
        return getVertexSize(root) - 1;
    }
};

vector<int> z_function(string &s)//, int &maxZ)
{
    int n = (int)s.length();
    vector<int> z(n, 0);
    z[0] = 0;
    for (int i = 1, l = 0, r = 0; i < n; ++i)
    {
        if (i <= r)
            z[i] = std::min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];
        if (i + z[i] - 1 > r)
        {
            l = i;
            r = i + z[i]-1;
        }
        //maxZ = std::max(maxZ, z[i]);
    }
    return z;
}

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    string str;
    cin >> str;

    bor mybor;

    int n = str.length();

    for (int j = 1; j < n; ++j)
    {
        string splitter  = str.substr(j) + '#' + str.substr(0, j);
        //cout << "splitter = '" << splitter << "'" << endl;
        int maxsuff = 0;
        vector <int> z = z_function(splitter);//, maxsuff);
        for (int i = str.length() - j; i < splitter.length(); ++i)
            maxsuff = std::max(maxsuff, z[i]);

        //cout << "my bor adding : '" << splitter.substr(0, maxsuff) << "'" << endl;

        mybor.add(splitter.substr(0, maxsuff));

    }
    //cout << "Now let's get size!" << endl;

    cout << mybor.getSize() << endl;

    //cout << "OK!" << endl;
    return 0;
}
