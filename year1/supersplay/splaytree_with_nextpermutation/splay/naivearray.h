#ifndef NAIVEARRAY_H
#define NAIVEARRAY_H

#include <vector>
#include <algorithm>
#include <iostream>

#include <array_interface.h>

class NaiveArray : public IArray
{
public:
    virtual DATA getElem(size_t index);
    virtual int  sum    (size_t from, size_t to);
    virtual void insert (size_t pos, DATA key);
    virtual void change (size_t pos, DATA newkey);
    virtual void remove (size_t pos);
    virtual void add    (size_t from, size_t to, DATA key);
    virtual void reverse(size_t from, size_t to);
    virtual void nextPermutation(size_t from, size_t to);

    virtual ~NaiveArray() {}

    virtual void print();
private:
    std::vector<DATA> array;
};

#endif // NAIVEARRAY_H

