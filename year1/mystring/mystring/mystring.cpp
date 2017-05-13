#include <mystring.h>

MyString::MyString(const char *str)
{
    int n = 0;
    for (n = 0; n < 1000; ++n)
        if (str[n] == '\0')
            break;

    s = new char[n];
    _len = n;
    for (int i = 0; i < n; ++i)
    {
        s[i] = str[i];
    }
}

MyString::~MyString()
{
    if (s) delete [] s;
}

char& MyString::operator [](const unsigned index)
{
    if (index > _len)
        assert("Out of bounds");

    return s[index];
}

bool MyString::operator>(MyString &b)
{
    unsigned int i = 0;
    while ((i < _len) && (i < b.len()))
    {
        if (s[i] > b[i])
            return true;
    }
    return (this->_len < b.len());
}

ostream &MyString::operator<<(ostream &out)
{
    for (unsigned int i = 0; i < _len; ++i)
        out << s[i];
    return out;
}

istream &MyString::operator>>(istream &in)
{
    char t;
    unsigned ii = 0;
    while (in >> t)
    {
        ++ii;
        if (ii >= _len)
        {
            char *newstring = new char [2*_len];
            for (unsigned int i = 0; i < _len; ++i)
                newstring[i] = s[i];

            delete [] s;
            s = newstring;
        }
        s[ii] = t;
    }
    return in;
}

MyString MyString::operator =(MyString &b)
{
    if (s) delete [] s;

    _len = b.len();
    s = new char [_len];
    for (unsigned int i = 0; i < _len; ++i)
        s[i] = b[i];\

    return (*this);
}
