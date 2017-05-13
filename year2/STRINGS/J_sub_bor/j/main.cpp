#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>
#include <cmath>

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

char tochrlet(int i)
{
    return (char)(i + 'a');
}

int tonumlet(char a)
{
    return (int)(a - 'a');
}

class bor
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

        vertex(vertex* newparent, char edgeChar)
        {
            child.assign(LETTERS_IN_THE_ALPHABET, nullptr);
            jumps.assign(LETTERS_IN_THE_ALPHABET, nullptr);
            parent = newparent;
            slink = up = nullptr;
            charToParent = edgeChar;
            leaf = false;
            numberTerm = 0;
            isNumberCounted = false;
        }
    };

    vertex* root;
    int n;
    vector<vector<int> > g;
    vector<int> used;
    int ans = 0;

    vertex * link(vertex* v, char c)
    {
        int t = tonumlet(c);
        //cout << "GETTING T = " << t << endl;

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

    vertex* getPrev(vertex* v)
    {
        if (v)
        {
            //cout << "ANUANUANUANSUANFS:BFLEWNBFLB:WEBFLHWEBFLHEWBFHFWE" << endl;
            if (v->up == nullptr)
            {
                if (v != root)
                {
                    //cout << "~~~~ANAL KARNAVAL~~~" << endl;
                    vertex * curslink = getSufLink(v);
                    if (curslink->leaf)
                        v->up = curslink;
                    else
                        v->up = getPrev(curslink);
                }
                else
                {
                    // cout << "CONDITION v == root is good now! " << endl;
                    v->up = nullptr;
                }
            }
            return v->up;
        }
        else
        {
            //cout << "ANUSANUSANUS" << endl;
            return nullptr;
        }
    }

    int getnLeaves(vertex *v)
    {
        if (v)
        {
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
            //cout << "fucking whore! returning" << endl;
            return v->numberTerm;
        }
        else
            return 0;
    }

    void dfs (int borv, vertex* autoVertex)
    {
        //cout << "Entered our dfs, with bv = " << borv << endl;

        used[borv] = 1;

        int curleavesstate = getnLeaves(autoVertex);
        ans += curleavesstate;

        for (int i = 0; i < LETTERS_IN_THE_ALPHABET; ++i)
        {
            int curv = g[borv][i];
            //cout << "looking for g["<<borv<<"][" << i << "] = "<< g[borv][i] <<" and used[" << g[borv][i] << "] = " << used[g[borv][i]] << endl;
            if (curv != 0 && used[curv] == 0)
            {
                //cout << "get there on i = " << i << endl;
                dfs(curv, link(autoVertex, tochrlet(i)));
            }
        }

        //cout << "LOOOOOOOOOOOOOOOOOOOOOOOOOOL" << endl;
        used[borv] = 2;
    }

public:
    bor()
    {
        ans = 0;
        root = new vertex(nullptr, tochr(0));
    }

    void add(string &s, int patternNumber)
    {
        //cout << "adding superstring! " << s << endl;
        vertex* iter = root;
        for (int i = 0; i < s.length(); ++i)
        {
            int t = tonumlet(s[i]);
            if (!iter->child[t])// || (!(iter->child[t] == iter)))
            {
                iter->child[t] = new vertex(iter, s[i]);
                //cout << "OK, CREATED THIS FUCKING NODE!" << endl;
            }
            iter = iter->child[t];
        }
        iter->leafPatternNumber.push_back(patternNumber);
        iter->leaf = true;
    }

    void scanBor(int n)
    {
        this->n = n;
        g.resize(n + 1);

        for (int i = 1; i <= n; ++i)
            g[i].resize(LETTERS_IN_THE_ALPHABET);

        int t;

        int aim;

        char c;
        for (int i = 1; i <= n; ++i)
        {
            cin >> t;
            for (int j = 0; j < t; ++j)
            {
                cin >> aim >> c;
                g[i][tonumlet(c)] = aim;
            }
        }
    }

    int searchWithDfs()
    {
        ans = 0;
        used.assign(n + 2, 0);

        this->dfs(1, root);

        return ans;
    }
};

int main()
{
    int n;
    cin >> n;

    bor mybor;
    mybor.scanBor(n);

    int m;
    cin >> m;
    for (int i = 0; i < m; ++i)
    {
        string s;
        cin >> s;
        mybor.add(s, i);

        // cout << mybor.getSize()<< endl;
    }


    cout << mybor.searchWithDfs();

    // cout << "Ok!!!" << endl;

    return 0;
}

