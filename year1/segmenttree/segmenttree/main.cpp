#include <segmenttree.h>

int main()
{
    int N;
    cin >> N;

    CSegmentTree tree(N);

    tree.getDataFromKeyboard();
    tree.build();

    int K;
    cin >> K;
    int C, D,
        R, G, B,
        E, F;

    for (int i = 0; i < K; i++)
    {
        cin >> C >> D >> R >> G >> B >> E >> F;

        tree.fill(C, D, R + G + B);

        cout << tree.getmin(E, F) << endl;
    }

    return 0;
}
