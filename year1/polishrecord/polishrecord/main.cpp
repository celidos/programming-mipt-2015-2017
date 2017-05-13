#include <iostream>

#include <polish_notation.h>
#include <string>

using namespace std;

int main()
{
    string req;
    getline(cin, req);

    bool ok;
    PolishRecordMaster M;
    float x = M.Resolve(req, ok);

    if (ok)
        cout << x;
    else
        cout << "error";
    cout << endl;

    return 0;
}
