#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdlib>

using std::cin;
using std::endl;
using std::cout;
using std::vector;
using std::string;
using std::atoi;

const size_t LETTERS_IN_THE_ALPHABET = 27;

class bor
{
private:
    struct vertex
    {
        int leaf;
        vector<vertex*> child;
        vector<int> childLeaf;

        vertex()
        {
            childLeaf.assign(LETTERS_IN_THE_ALPHABET, 0);
            child.assign(LETTERS_IN_THE_ALPHABET, nullptr);
            leaf = 0;
        }
    };

    int tonum(char a)
    {
        return (int) (a - 'a');
    }

    char tochr(int i)
    {
        return (char) (i + 'a');
    }

    vertex *root;

public:
    bor()
    {
        root = new vertex();
    }

    void add(string &s)
    {
        vertex *iter = root;
        for (size_t i = 0; i < s.size(); ++i)
        {
            int num = tonum(s[i]);

            iter->childLeaf[num]++;

            if (iter->child[num] == nullptr)
                iter->child[num] = new vertex();

            iter = iter->child[num];
        }
        iter->leaf++;
    }

    string getKstat(int k)
    {
        string s = "";

        int now = k;
        vertex *iter = root;
        while (true)
        {
            if (now <= iter->leaf)
                break;

            now -= iter->leaf;
            for (int i = 0; i < LETTERS_IN_THE_ALPHABET; ++i)
            {
                if (iter->childLeaf[i] >= now)
                {
                    s += tochr(i);
                    iter = iter->child[i];

                    break;
                }
                else
                    now -= iter->childLeaf[i];
            }
        }

        return s;
    }

};

int main()
{
    bor mybor;

    int N = 0;
    cin >> N;

    for (int i = 0; i < N; ++i)
    {
        string word;
        cin >> word;
        if (isalpha(word[0]))
            mybor.add(word);
        else
            cout << mybor.getKstat(atoi(word.c_str())) << "\n";
    }

    return 0;
}

