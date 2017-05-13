#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int N;
vector <vector <int> > a;
vector <vector <int> > b;

class myset
{
public:

    vector<int> v;
    void setSize( int newsize)
    {
        v.clear();
        for (int i = 0; i < newsize; ++i )
            v.push_back(0);
    }

    void enable(int index)
    {
        v[index] = 1;
    }

    void disable(int index)
    {
        v[index] = 0;
    }
    int size()
    {
        return v.size();
    }

    bool operator == (myset &b)
    {
        if (this->size() != b.size())
            return false;

        for (int i = 0; i < v.size(); ++i)
        {
            if (this->v[i] != b.v[i])
                return false;
        }
        return true;
    }

    void print()
    {
        cout << "[";
        for (int i = 0; i < v.size() - 1; ++i)
        {
            cout << v[i] << " ";
        }
        cout << v[v.size()-1] << "]";
    }
    void print_good()
    {
        cout << "[ ";
        for (int i = 0; i < v.size(); ++i)
        {
            if (v[i])
                cout << i + 1 << " ";
        }
        cout << "]";
    }
    bool inter(myset &b)
    {
        for (int i = 0; i < this->v.size(); ++i)
        {
            if (this->v[i] == 1 && b.v[i] == 1)
                return true;
        }
        return false;
    }
};



int main()
{
    cout << "Enter number of states: ";
    cin >> N;

    myset F;
    F.setSize(N);

    cout << "Enter mask of the endin' : ";

    for (int i = 0; i < N; ++i)
    {
        int temp;
        cin >> temp;

        if (temp) F.enable(i);
        else      F.disable(i);

        a.push_back(vector<int>());
        b.push_back(vector<int>());
    }

    cout << "Enter you graph in style (until !):\n\n  [letter] [edge_from] [edge_to]  \n  [letter] [edge_from] [edge_to]\n  ...\n\n";
    cout << ": ";
    char ch = 'a';
    int from, to;
    while (ch != '!')
    {
        cin >> ch;
        //from -= 1;
        //to -= 1;
        if (ch == 'a')
        {
            cin >> from >> to;
            a[from].push_back(to);

        }
        else if (ch == 'b')
        {
            cin >> from >> to;
            b[from].push_back(to);
         }
        else
            break;
    }

    // -----------------------------------------------------------------------------
    // DETERMININ'
    // -----------------------------------------------------------------------------

    cout << "DETERMINATED ----------------------------------------" << endl;

    bool flag = true;
    queue<myset> q;
    myset start;
    start.setSize(N);
    start.enable(0);
    q.push(start);

    vector<myset> setA;
    vector<myset> setB;

    vector <myset> havemet;
    while (!q.empty())
    {
        myset p = q.front();
        q.pop();

        bool outflag = false;
        for (int i = 0; i < havemet.size(); ++i)
        {
            if ((havemet[i] == p))
            {
                outflag = true;
                break;
            }
        }
        if (outflag)
            continue;

        p.print_good();
        flag = false;
        cout << "  >  ";
        havemet.push_back(p);

        // ------------------------------

        myset newA;
        newA.setSize(N);
        for (int i = 0; i < N; ++i)
        {
            if (p.v[i])
            {
                //cout << "pv[i="<<i<<"] = true";
                for (int j = 0; j < a[i].size(); ++j)
                {
                    //cout << "found a["<<i<<"]["<<j<<"] = "<< a[i][j] << endl;
                    newA.enable(a[i][j]);
                }
            }
        }
        newA.print_good();
        setA.push_back(newA);
        q.push(newA);

        cout << " | ";

        myset newB;
        newB.setSize(N);
        for (int i = 0; i < N; ++i)
        {
            if (p.v[i])
            {
                for (int j = 0; j < b[i].size(); ++j)
                {
                    newB.enable(b[i][j]);
                }
            }
        }
        newB.print_good();
        setB.push_back(newB);
        q.push(newB);
        cout << endl;

    }

    // ----------------------------------------------------------------------------------
    int M = havemet.size();

    myset newF;
    newF.setSize(M);
    for (int i = 0; i < M; ++i)
    {
         if (havemet[i].inter(F))
             newF.enable(i);
         else
             newF.disable(i);
    }

    cout << "NEW ENDIN' STATES ---------------------------------------------------------" << endl;
    newF.print_good();
    cout << endl;

    vector <int> jumpA, jumpB;
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            if (setA[i] == havemet[j])
            {
                jumpA.push_back(j);
                break;
            }
        }

        for (int j = 0; j < M; ++j)
        {
            if (setB[i] == havemet[j])
            {
                jumpB.push_back(j);
                break;
            }
        }
    }

    cout << "JUMPTABLE ----------------------------------------" << endl;
    for (int i = 0; i < M; ++i)
    {
        cout << i + 1 << "  >  " << jumpA[i]+1 << "; "<< jumpB[i]+1 << endl;
    }

    vector <unsigned int> oldtable;
    vector <unsigned int> newtable;
    int ii = 5;
  cout << "*** start iter"<< endl;
    for (int i = 0; i < M; ++i)
    {

        oldtable.push_back(newF.v[i]);
        cout <<i+1 << " : " <<  oldtable[i] << endl;
        newtable.push_back(0);
    }

    int power = 2;
    while (ii--)
    {
        //cout << "*** iter #" << endl;
        for (int i = 0; i < M; ++i)
        {
            newtable[i] = oldtable[jumpA[i]] * power + oldtable[jumpB[i]];
        //    cout << i+1 << " : " << newtable[i] << endl;
        }

        for (int i = 0; i < M; ++i)
            oldtable[i] = newtable[i];
        power *= power;

       /* int itt = 0;
                for (int i = 0; i < M; ++i)
                {
                    bool flagg = false;
                    for (int j = 0; j < i; ++j)
                        if (oldtable[i] == oldtable[j])
                        {
                            newtable[i] = newtable[j];
                            flagg = true;
                            break;
                        }

                    if (!flagg)
                    {
                        newtable[i] = itt;
                        ++itt;
                    }
                }*/
        cout << "FINAL VERSION ++++++++++++++++++++++++++++++++++++++" << endl;
        for (int i = 0; i < M; ++i)
        {
            cout << i+1 << " : " << oldtable[i]+1 << endl;
        }
    }

    int itt = 0;
            for (int i = 0; i < M; ++i)
            {
                bool flagg = false;
                for (int j = 0; j < i; ++j)
                    if (oldtable[i] == oldtable[j])
                    {
                        newtable[i] = newtable[j];
                        flagg = true;
                        break;
                    }

                if (!flagg)
                {
                    newtable[i] = itt;
                    ++itt;
                }
            }

    cout << "FINAL VERSION ++++++++++++++++++++++++++++++++++++++" << endl;
    for (int i = 0; i < M; ++i)
    {
        cout << i+1 << " : " << newtable[i]+1 << endl;
    }


}
