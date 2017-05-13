#include <iostream>
#include <vector>
#include <string>
#include <map>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::map;
using std::vector;

const size_t LETTERS_IN_THE_ALPHABET = 27;

int tonum(char a)
{
    return (int)(a - '0');
}

char tochr(int i)
{
    return (char)(i + '0');
}

class SuffixAuto
{
private:
    class vertex
    {
    public:
        bool leaf;
        vector<vertex*> child;

        vertex* parent;
        vertex* slink;
        vertex* up;
        bool isNumberCounted;
        int  numberTerm;
        char charToParent;
        vector<vertex*> jumps;
        vector<int> leafPatternNumber;

        vertex(vertex* par, char edgeChar)
        {
            child.assign(LETTERS_IN_THE_ALPHABET, nullptr);
            jumps.assign(LETTERS_IN_THE_ALPHABET, nullptr);
            parent = par;
            slink = up = nullptr;
            charToParent = edgeChar;
            leaf = false;
            numberTerm = 0;
            isNumberCounted = false;
        }
    };

    vertex *root;
    map<vertex *, int> used;

    vertex * link(vertex* v, char c)
    {
        int t = tonum(c);

        if (v->jumps[t] == nullptr)
        {
            // cout << "jumps[" << t << "] is free now!" << endl;
            if (v->child[t])
                v->jumps[t] = v->child[t];
            else if (v == root)
                v->jumps[t] = root;
            else
                v->jumps[t] = link(getSufLink(v), c);
        }
        return v->jumps[t];
    }

    vertex * getSufLink(vertex* v)
    {
        if (!v)
            return nullptr;

        if (v->slink == nullptr)
        {
            // cout << "THIS CASE!!!" << endl;
            if (v == root || v->parent == root)
                v->slink = root;
            else
                v->slink = link(getSufLink(v->parent), v->charToParent);
        }
        return v->slink;
    }

    int getnLeaves(vertex *v)
    {
        if (v == nullptr)
            return 0;

        // cout << "HERER!" << endl;
        if (!v->isNumberCounted)
        {
            //cout << "Entered this case!!" << endl;
            v->isNumberCounted = true;

            if (v == root)
                v->numberTerm = 0;
            else
            {
                // our << "v->leaf = " << (v->leaf == nullptr) << endl;
                if (v->leaf)
                    v->numberTerm += v->leafPatternNumber.size();

                v->numberTerm += getnLeaves(getSufLink(v));
            }
        }
        return v->numberTerm;
    }

    bool dfs(vertex* v)
    {
        used[v] = 1;

        for (int i = 0; i < 2; ++i)
        {
            // cout << "dfs for i = " << i << endl;
            vertex* next = link(v, tochr(i));
            if (used.count(next) == 0 && getnLeaves(next) == 0)
            {
                if (dfs(next))
                {
                    // cout << "DFS FINISHED it's work!" << endl;
                    return true;
                }
            }
            else if (getnLeaves(next) == 0 && used[next] == 1)
                return true;
        }

        used[v] = 2;
        return false;
    }

public:
    SuffixAuto()
    {
        //cout << "constructor called!" << endl;
        root = new vertex(nullptr, '0');
    }

    void add(string &s, int patternNumber)
    {
        // cout << "adding superstring! " << s << endl;
        vertex* iter = root;
        for (size_t i = 0; i < s.length(); ++i)
        {
            int t = tonum(s[i]);
            if (!iter->child[t])// || (!(iter->child[t] == iter)))
                iter->child[t] = new vertex(iter, s[i]);

            iter = iter->child[t];
        }
        iter->leafPatternNumber.push_back(patternNumber);
        iter->leaf = true;
    }

    bool hasCycles()
    {
        return dfs(root);
    }
};

int main()
{
    int N;
    cin >> N;

    SuffixAuto sufAuto;

    for (int i = 0; i < N; ++i)
    {
        string str;
        cin >> str;
        sufAuto.add(str, i);
    }

    if (sufAuto.hasCycles())
        cout << "TAK" << endl;
    else
        cout << "NIE" << endl;

    //cout << "OK!" << endl;

    return 0;
}
