#ifndef ARRAY_INTERFACE
#define ARRAY_INTERFACE

typedef int DATA;

class IArray
{
public:
    virtual DATA getElem(size_t index) = 0;
    virtual int  sum    (size_t from, size_t to) = 0;
    virtual void insert (size_t pos, DATA key) = 0;
    virtual void change (size_t pos, DATA newkey) = 0;
    virtual void remove (size_t pos) = 0;
    virtual void add    (size_t from, size_t to, DATA key) = 0;
    virtual void reverse(size_t from, size_t to) = 0;
    virtual void nextPermutation(size_t from, size_t to) = 0;

    virtual void print() = 0;

    virtual ~IArray() {}
};

#endif // ARRAY_INTERFACE

