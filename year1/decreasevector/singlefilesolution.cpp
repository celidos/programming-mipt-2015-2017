

#ifndef MYSTACK
#define MYSTACK

#include <iostream>
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


#ifndef MYDEQUE
#define MYDEQUE

template< typename T >
class mydeque
{
private:
    mystack<T> instack;
    mystack<T> outstack;
    uint size;

public:
    mydeque() : size(0) {};
    ~mydeque() {};

    void       push_back(const T& element);
    const T pop_front();
    bool       empty();
};


template< typename T >
void mydeque<T>::push_back(const T& element)
{
    instack.push_back(element);
    ++size;
}

template< typename T >
const T mydeque<T>::pop_front()
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
bool mydeque<T>::empty()
{
    return (size == 0);
}

#endif // MYDEQUE

using std::cin;
using std::cout;
using std::endl;

int main()
{
    int N;
    cin >> N;

    mydeque <int> deq;

    int a, b;
    bool still_ok_flag = true;
    for (int i = 0; i < N; ++i)
    {
        cin >> a >> b;

        if (a == 3)
            deq.push_back(b);
        else
        {
            int x = -1;
            if (!deq.empty())
                x = deq.pop_front();
            if (x != b)
                still_ok_flag = false;
        }
    }

    cout << (still_ok_flag ? "YES" : "NO") << endl;

    return 0;
}

