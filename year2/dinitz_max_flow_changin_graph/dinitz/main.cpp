#include <iostream>

#include "dinitz.h"

int main()
{
    //cout << "Hi." << endl;
    Graph g;
    g.readFromKeyboard();
    //cout << "reading finished" << endl;

    std::cout << g.dinitz() << std::endl;

    int countOfChanges;
    cin >> countOfChanges;
    for (int temp = 0; temp < countOfChanges; ++temp)
    {
        int plus, from, to;
        cin >> plus >> from >> to;
        from -= 1;
        to -= 1;
        plus = (plus == 1 ? 1 : -1);

        cout <<  g.changeEdge(from, to, plus) << endl;
    }

    //std::cout << "Hello, world!" << std::endl;

    return 0;
}
