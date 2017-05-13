#ifndef MYSTRING
#define MYSTRING

#include <assert.h>
#include <iostream>

using std::ostream;
using std::istream;

class MyString
{
private:
    char * s;
    unsigned _len;
public:
    MyString(const char *str);
    ~MyString();

    unsigned int len() {return _len;}

    char& operator[] (unsigned index);
    MyString operator= (MyString& b);
    bool operator> (MyString& b);

    ostream& operator<< (ostream& out);
    istream &operator>> (istream& in);
};

#endif // MYSTRING

