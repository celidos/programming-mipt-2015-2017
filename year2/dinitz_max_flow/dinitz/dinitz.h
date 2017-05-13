#ifndef DINITZ_H
#define DINITZ_H

#include <iostream>
#include <vector>
#include <queue>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::queue;

const int INF = 1234000010;

class Graph
{
public:
    Graph() {}
    ~Graph() {}

    bool readFromKeyboard();
    int dinitz();
private:
    int N;
    int start;
    int finish;

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

    bool bfs();
    int dfs(int v, int flow);
};


bool Graph::readFromKeyboard()
{
    cin >> N;

    if (N == 0)
        return false;

    for (int i = 0; i < N; ++i)
    {
        edge.push_back(vector<Edge>());
        level.push_back(-1);
        useless_prefix.push_back(0);
    }

    cin >> start >> finish;
    start -= 1;
    finish -= 1;
    int edgesCount;
    cin >> edgesCount;


    //cout << endl << "EDGES COUNT : " << edgesCount << endl;

    for (int temp1 = 0; temp1 < edgesCount; ++temp1)
    {
        //cout << "!!!!!" << endl;

        int from, to, newc;
        cin >> from >> to >> newc;
        from -= 1;
        to -= 1;

        bool flag = false;
        //for (int i = 0; i < edge[from].size(); ++i)
        //    if (edge[from][i].aim == to)
        //    {
        //        edge[from][i].c += newc;
        //        flag = true;
        //    }

        //cout << "FLAG = " << flag << endl;

        if (!flag)
        {
            //cout << "Pushing ["<<from<<"]["<<to<<"]; newc = " << newc << "; back = " << edge[to].size() <<  endl;
            edge[from].push_back(Edge(to, newc, 0, edge[to].size()));
            //cout << "Pushing ["<<to<<"]["<<from<<"]; newc = " << 0 << "; back = " << edge[from].size() - 1 <<  endl;
            edge[to].push_back(Edge(from, newc, 0, edge[from].size() - 1));

        }
    }
    return true;
}

int Graph::dinitz()
{
    int maxflow = 0;
    while (true)
    {
        //cout << "ok!" << endl;
        if (!bfs())
            break;
        //cout << "even here " << endl;

        int pushed = 1;
        while (pushed)
        {
            pushed = dfs(start, INF);
            maxflow += pushed;
        }
    }
    return maxflow;
}

bool Graph::bfs()
{
    queue <int> q;
    q.push(start);
    for (int i = 0; i < N; ++i)
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
    //cout << "flow mmmmmmmmmmm = " << flow << endl;


    if (!flow)
        return 0;

    if (v == finish)
        return flow;

    for (int i = useless_prefix[v]; i < edge[v].size(); ++i)
    {
        //cout << "i = " << i << endl;
        int newv = edge[v][i].aim;

        if (level[newv] != level[v] + 1)
        {
            useless_prefix[v] = i;
            continue;
        }
//cout << "passed continue"<< endl;
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
