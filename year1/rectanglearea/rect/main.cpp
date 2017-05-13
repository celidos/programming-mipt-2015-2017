#include <iostream>
#include <stack>

using std::cin;
using std::cout;
using std::endl;
using std::stack;

struct rect
{
    int w, h,
        lindex, rindex,
        part_sum;
    void set(int W, int H)
    {
        w = W;
        h = H;
    }
};

int find_max_square(rect *aRect, int N)
{
    aRect[0]  .set(-1, -1);
    aRect[N+1].set(-1, -1);

    stack <int> stack_r;                    // находим минимальный справа
    for (int i = 1; i <= N + 1; ++i)
    {
        while (!stack_r.empty() && aRect[stack_r.top()].h > aRect[i].h)
        {
            aRect[stack_r.top()].rindex = i;
            stack_r.pop();
        }
        stack_r.push(i);
    }

    stack <int> stack_l;                    // Находим минимальный слева
    for (int i = N; i >= 0; --i)
    {
        while (!stack_l.empty() && aRect[stack_l.top()].h > aRect[i].h)
        {
            aRect[stack_l.top()].lindex = i;
            stack_l.pop();
        }
        stack_l.push(i);
    }

    // Насчитываем частичные суммы

    aRect[0].part_sum = 0;
    for (int i = 1; i <= N; ++i)
        aRect[i].part_sum = aRect[i-1].part_sum + aRect[i].w;

    // Ищем теперь максимальную площадь

    int max_square = 0;
    for (int i = 1; i <= N; ++i)
    {
        int L = aRect[i].lindex, R = aRect[i].rindex;
        int t = aRect[i].h * (aRect[R - 1].part_sum - aRect[L].part_sum);
        if (t > max_square)
            max_square = t;
    }

    return max_square;
}

int main()
{
    int N;
    cin >> N;

    rect *A = new rect [N+2];

    for (int i = 1; i <= N; ++i)
        cin >> A[i].w >> A[i].h;

    cout << find_max_square(A, N) << endl;

    delete [] A;

    return 0;
}
