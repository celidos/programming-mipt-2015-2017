#ifndef DINITZ_H
#define DINITZ_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::queue;

const int INF = 1234000010;

class Graph
{
public:
    Graph() {maxflow = 0;}
    ~Graph() {}

    void readPairsFromKeyboard();
    void readField();
    int  dinitz();
private:
    int size;
    int n;
    int m;
    int a;
    int b;
    int start;
    int finish;

    int maxflow;

    struct Edge
    {
        int aim;
        int c;
        int f;
        int back;

        Edge() {}
        Edge(int _aim, int _c, int _f, int _back)
        {
            aim = _aim; c = _c; f = _f; back = _back;
        }
    };

    vector <vector <Edge> > edge;
    vector <int> level;
    vector <int> useless_prefix;


    void addDirectEdge(int from, int to, int C);
    void addUndirectEdge(int from, int to, int C);
    bool bfs();
    int dfs(int v, int flow);
};

void Graph::readField()
{
    cin >> n >> m >> a >> b;
    vector <vector <char> > data;
    int it = 0;
    int counterBlack = 0, counterWhite = 0;

    data.push_back(vector<char>(m+2, 0));

    for (int i = 1; i <= n; ++i)
    {
        data.push_back(vector<char>());
        data[i].push_back(0);
        for (int j = 1; j <= m; ++j)
        {
            //cout << "i = " << i << "; j = " << j << endl;
            it = 1 - it;
            char t;
            cin >> t;
            if (t == '*')
            {
                data[i].push_back(1);
                if (!it)
                    ++counterWhite;
                else
                    ++counterBlack;
            }
            else
                data[i].push_back(0);
        }
        data[i].push_back(0);
    }
    data.push_back(vector<char>(m+2, 0));

    // ---------------------------------------------

    /*for (int i = 0; i < data.size(); ++i)
    {
        for (int j = 0; j < data[i].size(); ++j)
        {
            cout << (int)(data[i][j]) << " ";
        }
        cout << endl;
    }*/

    // ---------------------------------------------

    //cout << "black = " << counterBlack << "; white = " << counterWhite << endl;

    if (2*b < a)
    {
        cout << (counterBlack + counterWhite) * b << endl;
        return;
    }
    else
    {
        size = n * m + 2;
        start = n * m;
        finish = n*m+1;

        for (int i = 0; i < size; ++i)
        {
            edge.push_back(vector<Edge>());
            level.push_back(-1);
            useless_prefix.push_back(0);
        }

        for (int i = 1; i <= n; ++i)
        {
            for (int j = 1; j <= m; ++j)
            {
                int ii = i - 1;
                int jj = j - 1;

                if ((i + j) % 2 && data[i][j])
                {
                    //cout << "PRINTIN for ii = " <<i -1<<"; jj = " << j - 1 << endl;

                    if (data[i+1][j])
                    {
                        addDirectEdge(ii*m+jj, (ii+1)*m+jj, 1);
                        //cout << "case A, " << ii+1 << "; " << jj << endl;
                    }
                    if (data[i-1][j])
                    {
                        addDirectEdge(ii*m+jj, (ii-1)*m+jj, 1);
                        //cout << "case B, " << ii-1 << "; " << jj << endl;
                    }
                    if (data[i][j+1])
                    {
                        addDirectEdge(ii*m+jj, ii*m+jj+1, 1);
                        //cout << "case C, " << ii << "; " << jj+1 << endl;
                    }
                    if (data[i][j-1])
                    {
                        addDirectEdge(ii*m+jj, ii*m+jj-1, 1);
                        //cout << "case D, " << ii << "; " << jj -1<< endl;
                    }


                    addDirectEdge(n*m, ii*m+jj, 1);
                }
                else
                    addDirectEdge(ii*m+jj, m*n+1, 1);
            }
        }

        // ------------------------------------------------------

        /*for (int to = 1; to < n*m; to+=2)
        {
            addDirectEdge(n*m, to, 1);
        }

        for (int from = 0; from < m *n; from+=2)
        {
            addDirectEdge(from, m*n+1, 1);
        }*/

        // ------------------------------------------------------

        //cout << "LET's PRINT OUR GRAPH!!!" << endl;
        /*for (int i = 0; i < size+2; ++i)
        {
            //cout << "For v = " << i << " : ";
            for (int j = 0; j < edge[i].size(); ++j)
            {
                cout << edge[i][j].aim << "("<<edge[i][j].f<<"/"<<edge[i][j].c<<") ";
            }
            cout << endl;
        }*/

        // ------------------------------------------------------
        int din = dinitz();

        //cout << "DIN = " << din << endl;
        cout << din * a + (counterBlack + counterWhite - 2*din) * b << endl;
    }
}

int Graph::dinitz()
{
    int maxflow = 0;
    for (int i = 0; i < size; ++i)
    {
        level[i] = -1;
        useless_prefix[i] = 0;
    }

    while (true)
    {
        if (!bfs())
            break;

        int pushed = 1;
        while (pushed)
        {
            pushed = dfs(start, INF);
            maxflow += pushed;
        }
    }
    return maxflow;
}

void Graph::addDirectEdge(int from, int to, int C)
{
    edge[from].push_back(Edge(to,   C, 0, edge[to  ].size()));
    edge[to  ].push_back(Edge(from, 0, 0, edge[from].size() - 1));
}

void Graph::addUndirectEdge(int from, int to, int C)
{
    edge[from].push_back(Edge(to,   C, 0, edge[to  ].size()));
    edge[to  ].push_back(Edge(from, C, 0, edge[from].size() - 1));
}

bool Graph::bfs()
{
    queue <int> q;
    q.push(start);
    for (int i = 0; i < size; ++i)
    {
        level[i] = -1;
        useless_prefix[i] = 0;
    }

    level[start] = 0;

    while (!q.empty())
    {
        int top = q.front();
        //cout << "POPPPED : " << top << endl;
        q.pop();

        for (int i = 0; i < edge[top].size(); ++i)
        {
            int newv = edge[top][i].aim;
            //cout << "-----friend : " << newv << "; lvl = " << level[newv] << "; f = " <<edge[top][i].f << "; c = " << edge[top][i].c << "; back = " << edge[top][i].back << endl;
            if (level[newv] == -1 && edge[top][i].f < edge[top][i].c)
            {
                level[newv] = level[top] + 1;
                //cout << "lvl[" << newv << "] = lvl[" << top << "] + 1 = " << level[newv] << endl;
                q.push(newv);
            }
        }
    }
    //cout << "WE GOT LEVEL[finish] = " << level[finish] << endl;

    return level[finish] != -1;
}

int Graph::dfs(int v, int flow)
{
    if (!flow)
        return 0;

    if (v == finish)
        return flow;

    for (int i = useless_prefix[v]; i < edge[v].size(); ++i)
    {
        int newv = edge[v][i].aim;

        if (level[newv] != level[v] + 1)
        {
            useless_prefix[v] = i;
            continue;
        }
        int pushed = dfs(newv, std::min(flow, edge[v][i].c - edge[v][i].f));
        if (pushed)
        {
            edge[v][i].f += pushed;
            edge[newv][edge[v][i].back].f -= pushed;
            return pushed;
        }
        else
            useless_prefix[v] = i;
    }
    return 0;
}

#endif // DINITZ_H
