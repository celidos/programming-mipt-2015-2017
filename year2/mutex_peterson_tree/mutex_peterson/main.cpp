#include <iostream>

using std::cout;

#include "tree_mutex.h"

int main(int argc, char *argv[])
{
    tree_mutex tree(1);

    tree.lock(0);
    //cout << endl;
    tree.unlock(0);
    cout << "OK, done!\n";

    //cout << "Hello World!\n";
    return 0;
}

