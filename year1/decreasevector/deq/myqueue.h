#ifndef MYDEQUE
#define MYDEQUE

#include <mystack.h>

template< typename T >
class myqueue
{
private:
    mystack<T> instack;
    mystack<T> outstack;
    uint size;

public:
    myqueue() : size(0) {};
    ~myqueue() {};

    void       push_back(const T& element);
    const T pop_front();
    bool       empty();
};


template< typename T >
void myqueue<T>::push_back(const T& element)
{
    instack.push_back(element);
    ++size;
}

template< typename T >
const T myqueue<T>::pop_front()
{
    if (size > 0)
    {
        --size;

        if (!outstack.empty())
            return outstack.pop_back();
        else
        {
            while (!instack.empty())
                outstack.push_back(instack.pop_back());

            return outstack.pop_back();
        }
    }
    assert("mydeque::pop_front : Deque is empty");
}

template< typename T >
bool myqueue<T>::empty()
{
    return (size == 0);
}

#endif // MYDEQUE

