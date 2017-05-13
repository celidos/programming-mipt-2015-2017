#include <iostream>

#include <mydeque.h>

int main()
{
    mydeque <int> deq;

    int N;
    cin >> N;
    /*int t;
    for (int i = 0; i < 100; ++i)
    {
        cin >> t;
        dq.push_front(t);
        dq.print_deque();
    }*/

    int a, b;
    bool still_ok_flag = true;
    for (int i = 0; i < N; ++i)
    {
        cin >> a >> b;

        if (a == 1)
        {
            deq.push_front(b);
            deq.print_deque();
        }
        if (a == 3)

        {
            deq.push_back(b);
            deq.print_deque();
        }
        else if (a == 2)
        {
            int x = -1;
            if (!deq.empty())
                x = deq.pop_front();
            cout <<" >>>> " << x << endl;
            if (x != b)
                still_ok_flag = false;
        }
        else if (a == 4)
        {
            int x = -1;
            if (!deq.empty())
                x = deq.pop_back();
            cout <<" >>>> " << x << endl;

            if (x != b)
                still_ok_flag = false;
        }
    }

    cout << (still_ok_flag ? "YES" : "NO") << endl;

    return 0;
}
