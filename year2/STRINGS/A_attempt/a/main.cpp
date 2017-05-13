#include <bits/stdc++.h>
#define clr(x) memset((x), 0, sizeof(x))
#define all(x) (x).begin(), (x).end()
#define pb push_back
#define mp make_pair
#define For(i, st, en) for(int i=(st); i<=(int)(en); i++)
#define Ford(i, st, en) for(int i=(st); i>=(int)(en); i--)
#define forn(i, n) for(int i=0; i<(int)(n); i++)
#define ford(i, n) for(int i=(n)-1; i>=0; i--)
#define fori(it, x) for (__typeof((x).begin()) it = (x).begin(); it != (x).end(); it++)
#define in(x) int (x); input((x));
#define x first
#define y second
#define less(a,b) ((a) < (b) - EPS)
#define more(a,b) ((a) > (b) + EPS)
#define eq(a,b) (fabs((a) - (b)) < EPS)
#define remax(a, b) ((a) = (b) > (a) ? (b) : (a))
#define remin(a, b) ((a) = (b) < (a) ? (b) : (a))
using namespace std;
typedef long double ld; template <class _T> inline _T sqr(const _T& x) {return x * x;} template <class _T> inline string tostr(const _T& a) {ostringstream os(""); os << a; return os.str();} const ld PI = 3.1415926535897932384626433832795L; const ld EPS = 5e-12; char TEMPORARY_CHAR; typedef long long ll; typedef unsigned long long ull; typedef set < int > SI; typedef vector < int > VI; typedef vector < vector < int > > VVI; typedef map < string, int > MSI; typedef pair < int, int > PII; const int INF = 1e9; inline void input(int &a) {a = 0; while (((TEMPORARY_CHAR = getchar()) > '9' || TEMPORARY_CHAR < '0') && (TEMPORARY_CHAR != '-')){} char neg = 0; if (TEMPORARY_CHAR == '-') {neg = 1; TEMPORARY_CHAR = getchar();} while (TEMPORARY_CHAR <= '9' && TEMPORARY_CHAR >= '0') {a = 10 * a + TEMPORARY_CHAR - '0'; TEMPORARY_CHAR = getchar();} if (neg) a = -a;} inline void out(int a) {if (!a) putchar('0'); if (a < 0) {putchar('-'); a = -a;} char s[10]; int i; for(i = 0; a; ++i) {s[i] = '0' + a % 10; a /= 10;} ford(j, i) putchar(s[j]);} inline int nxt() {in(ret); return ret;}

#define bit(a, pos) ((a >> pos) & 1)

struct state
{
    int len;
    state * link;
    state * next[10];
    int cnt;
    char can;
    state()
    {
        cnt = 0;
        len = 0;
        link = 0;
        can = -1;
        memset(next, 0, sizeof(next));
    }

};

state * root;
state * last;

void init_sa()
{
    root = new state();
    last = root;
}

vector <state *> pos[150001];

void extend_sa(int c)
{
    state * cur = new state();
    cur->len = last->len + 1;
    pos[cur->len].pb(cur);
    cur->cnt = 1;
    state * p = last;
    for(; p && !p->next[c]; p = p->link)
        p->next[c] = cur;
    if (!p)
        cur->link = root;
    else
    {
        state * q = p->next[c];
        if (p->len + 1 == q->len)
            cur->link = q;
        else
        {
            state * clone = new state();
            clone->len = p->len + 1;
            pos[clone->len].pb(clone);
            memcpy(clone->next, q->next, sizeof(q->next));
            clone->link = q->link;
            for(; p && p->next[c] == q; p = p->link)
                p->next[c] = clone;
            q->link = cur->link = clone;
        }
    }
    last = cur;
}

ll best = 0;
state * bp;

char can(state * v)
{
    if (!v) return 0;
    if (v->can != -1)
        return v->can;
    return v->can = can(v->link);
}

int main()
{
    in(n); in(m);
    int a[n];
    init_sa();
    forn(i, n)
    {
        a[i] = nxt() - 1;
        extend_sa(a[i]);
    }
    for(int i = n; i > 0; --i)
        for(int j = 0; j < (int)pos[i].size(); ++j)
        {
            state * x = pos[i][j];
            if (x->link) x->link->cnt += x->cnt;
            ll cur = x->len * 1LL * x->cnt;
            if (cur > best)
            {
                best = cur;
                bp = x;
            }
        }
    bp->can = 1;
    cout << best << endl;
    cout << bp->len << endl;
    state * cur = root;
    for(int i = 0; i < n; ++i)
    {
        cur = cur->next[a[i]];
        if (can(cur))
        {
            for(int j = 0; j < bp->len; ++j)
            {
                if (j) cout << " ";
                cout << a[i - bp->len + j + 1] + 1;
            }
            cout << endl;
            return 0;
        }
    }
    return 0;

}
