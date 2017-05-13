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
    int dinitz();
    void getMaxFriends();
private:
    int size;
    int n;
    int m;
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
    vector <vector <int> > g;
    vector <int> level;
    vector <int> useless_prefix;
    vector <bool> visited;
    vector <bool> isfree;

    bool bfs();
    int dfs(int v, int flow);
    void dfs_simple(int v);
};

void Graph::readPairsFromKeyboard()
{
    cin >> m >> n;
    size = n + m + 2;

    start = size - 2;
    finish = size - 1;

    for (int i = 0; i < size; ++i)
    {
        edge.push_back(vector<Edge>());
        level.push_back(-1);
        useless_prefix.push_back(0);
    }

    for (int from = 0; from < m; ++from)
    {
        vector <int> temp;

        while (true)
        {
            int to = -1;
            cin >> to;
            if (to == 0)
                break;

            temp.push_back(to - 1);
        }

        sort(temp.begin(), temp.end());
        temp.push_back(-42);

        int it = 0;
        for (int to = 0; to < n; ++to)
        {
            if (to == temp[it])
            {
                ++it;
                continue;
            }
            else
            {
                edge[from].push_back(Edge(m+to, 1, 0, edge[m+to].size()));
                //cout << "Pushing ["<<to<<"]["<<from<<"]; newc = " << 0 << "; back = " << edge[from].size() - 1 <<  endl;
                edge[m+to].push_back(Edge(from, 0, 0, edge[from].size() - 1));
            }
        }

    }

    // ----------------------------------------------------------

    for (int to = 0; to < m; ++to)
    {
        edge[size - 2].push_back(Edge(to, 1, 0, edge[to].size()));
        edge[to].push_back(Edge(size - 2, 0, 0, edge[size - 2].size() - 1));
    }

    for (int from = m; from < m + n; ++from)
    {
        edge[from].push_back(Edge(size - 1, 1, 0, edge[size - 1].size()));
        edge[size - 1].push_back(Edge(from, 0, 0, edge[from].size() - 1));
    }

    // --------------------------------------------------

    /*cout << "LET's PRINT OUR GRAPH!!!" << endl;
    for (int i = 0; i < size; ++i)
    {
        cout << "For v = " << i << " : ";
        for (int j = 0; j < edge[i].size(); ++j)
        {
            cout << edge[i][j].aim << "("<<edge[i][j].f<<"/"<<edge[i][j].c<<") ";
        }
        cout << endl;
    }*/
}

int Graph::dinitz()
{
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

void Graph::getMaxFriends()
{
    dinitz();

    // ---------------------------------------------------------------

    for (int i = 0; i < size - 2; ++i)
    {
        g.push_back(vector<int>());
        visited.push_back(false);
        isfree.push_back(true);
    }

    for (int from = 0; from < size - 2; ++from)
    {
        //cout << "from = " << from << endl;
        for (int i = 0; i < edge[from].size(); ++i)
        {
            //cout << "looking for edge (" << from << "; " << edge[from][i].aim << ")" << endl;
            if (edge[from][i].aim < size - 2 && edge[from][i].c == 1)
            {
                if (edge[from][i].f == 0)
                {
                    g[from].push_back(edge[from][i].aim);
                    //isfree[edge[from][i].aim] = false;
                    //cout << "_EDGE from " << from << " to " << edge[from][i].aim << endl;
                }
                else if (edge[from][i].f == 1)
                {
                    g[edge[from][i].aim].push_back(from);
                    isfree[from]=false;
                    //cout << "EDGE from " << edge[from][i].aim << " to " << from << endl;
                }
            }
        }
    }


    // ---------------------------------------------------------------

    for (int i = 0; i < m; ++i)
    {
        if (!visited[i] && isfree[i])
            dfs_simple(i);
    }

    // ---------------------------------------------------------------

    //cout << "PRINTING RESULTS !" << endl;

    vector <int> men;
    vector <int> women;
    for (int i = 0; i < m; ++i)
    {
        if (visited[i])
            men.push_back(i+1);
    }

    for (int i = m; i < m+n; ++i)
    {
        if (!visited[i])
            women.push_back( i - m + 1);//<< " ";
    }
    sort(men.begin(), men.end());
    sort(women.begin(), women.end());
//cout << "SIZE  = ";
    cout << men.size() + women.size() << endl;
    cout << men.size() << " " << women.size() << endl;
    //cout << "MEN : ";
    for (int i = 0; i < men.size(); ++i)
    {
        cout << men[i] << " ";
    }
    cout << endl;
    //cout << "WOMEN : ";
    for (int i = 0; i < women.size(); ++i)
    {
        cout << women[i] << " ";
    }

    cout << endl << endl;
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

void Graph::dfs_simple(int v)
{
    if (visited[v])
        return;

    visited[v] = true;
    for (int i = 0; i < g[v].size(); ++i)
    {
        if (!visited[g[v][i]])
        {
            dfs_simple(g[v][i]);
        }
    }
}

#endif // DINITZ_H
