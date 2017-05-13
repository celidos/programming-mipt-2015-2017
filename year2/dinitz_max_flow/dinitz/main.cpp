#include <iostream>

#include "dinitz.h"

int main()
{
    //cout << "Hi." << endl;
    while (1)
    {
        Graph g;
        if (g.readFromKeyboard())
            std::cout << g.dinitz() << std::endl;
        else
            break;
    }

    std::cout << "Hello, world!" << std::endl;

    return 0;
}
