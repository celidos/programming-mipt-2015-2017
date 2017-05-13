#include <iostream>
#include <myqueue.h>

using std::cin;
using std::cout;
using std::endl;

int main()
{
    int N;
    cin >> N;

    myqueue <int> que;

    int a, b;
    bool still_ok_flag = true;
    for (int i = 0; i < N; ++i)
    {
        cin >> a >> b;

        if (a == 3)
            que.push_back(b);
        else
        {
            int x = -1;
            if (!que.empty())
                x = que.pop_front();
            if (x != b)
                still_ok_flag = false;
        }
    }

    cout << (still_ok_flag ? "YES" : "NO") << endl;

    return 0;
}
