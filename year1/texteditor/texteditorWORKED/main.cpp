#include <iostream>
#include <texteditor.h>

using namespace std;

int main()
{
    TextEditor teMaster;
    teMaster.Solve("test.txt");
    cout << "OK!"<< endl;

    return 0;
}
