#ifndef MYSTACK
#define MYSTACK

#include <vector>
#include <assert.h>

using std::vector;

typedef unsigned int uint;

template< typename T >
class mystack
{
private:
    vector <T> data;
    uint size;
public:
    mystack() : size(0) {};
    ~mystack() {};

    void       push_back(const T& element);
    const T    pop_back();
    T&         operator[] (uint index);

    bool       empty();
};

template< typename T >
void mystack<T>::push_back(const T &element)
{
    data.push_back(element);
    ++size;
}

template< typename T >
const T mystack<T>::pop_back()
{
    if (size > 0)
    {
        T result = data[size - 1];
        data.pop_back();
        --size;
        return result;
    }
    assert("mystack::pop_back : Stack is empty!");
}

template< typename T >
T &mystack<T>::operator[](uint index)
{
    if (index < size)
        return data[index];
    assert("mystack::operator[] : Out of bounds");
}

template< typename T >
bool mystack<T>::empty()
{
    return (size == 0);
}

#endif // MYSTACK
