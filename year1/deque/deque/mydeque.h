#ifndef MYDEQUE
#define MYDEQUE

#include <vector>
#include <iostream>
#include <assert.h>

using std::vector;
using std::cin;
using std::cout;
using std::endl;

typedef unsigned int uint;

const uint START_SIZE_OF_VECTOR = 4;

template< typename T >
class mydeque
{
private:
    uint _sizeofmemory;     // Размер выделенной памяти
    uint _sizeofdata;       // Количество уже существующих элементов
    vector <T> _data;       // Буфер
    int l;
    int r;

    void       extend_memory();     // Увеличивает размер массива в 2 раза
    void       reduce_memory();     // Уменьшает размер массива в 2 раза
public:
    mydeque();
    ~mydeque() {};

    void       push_back (const T& element);
    void       push_front(const T& element);
    const T    pop_back();
    const T    pop_front();
    T&         operator[] (int index);

    void       back(const T& element);
    const T    back() const;
    void       front(const T& element);
    const T    front() const;

    void       print_deque() const;

    bool       empty() const;
    uint       sizeofmemory() const;
};

template< typename T >
void mydeque<T>::extend_memory()
{
    _data.resize(2*_sizeofmemory);

    for (int i = l; (i % _sizeofmemory) != r; ++i)
        _data[i] = _data[i % _sizeofmemory];

    if (r < l)
        r += _sizeofmemory;

    _sizeofmemory *= 2;
}

template< typename T >
void mydeque<T>::reduce_memory()
{
    vector<T> buffer(_sizeofmemory / 2);

    for (int i = l; (i % _sizeofmemory) != r; ++i)
        buffer[i - l] = _data[i % _sizeofmemory];

    _data = buffer;
    l = 0;
    r = _sizeofdata;
    _sizeofmemory /= 2;
}

template< typename T >
mydeque<T>::mydeque()
    : l(0), r(0), _sizeofmemory(START_SIZE_OF_VECTOR),
      _data(vector<T> (_sizeofmemory)), _sizeofdata(0)
{
}

template< typename T >
void mydeque<T>::push_back(const T &element)
{
    if (_sizeofmemory == _sizeofdata + 1) // нужно больше памяти
        extend_memory();

    l = (_sizeofmemory - 1 + l) % _sizeofmemory;
    _data[l] = element;
    ++_sizeofdata;
}

template< typename T >
void mydeque<T>::push_front(const T &element)
{
    if (_sizeofmemory == _sizeofdata + 1) // нужно больше памяти
        extend_memory();

    _data[r] = element;
    r = (r + 1) % _sizeofmemory;
    ++_sizeofdata;
}

template< typename T >
const T mydeque<T>::pop_back()
{
    T ans = _data[l];
    l = (l + 1) % _sizeofmemory;
    --_sizeofdata;

    if (_sizeofdata*4 < _sizeofmemory && _sizeofmemory > START_SIZE_OF_VECTOR)
        reduce_memory();

    return ans;
}

template< typename T >
const T mydeque<T>::pop_front()
{
    T ans = _data[(r - 1 + _sizeofmemory) % _sizeofmemory];
    r = (r - 1 + _sizeofmemory) % _sizeofmemory;
    --_sizeofdata;

    if (_sizeofdata*4 < _sizeofmemory && _sizeofmemory > START_SIZE_OF_VECTOR)
        reduce_memory();

    return ans;
}

template< typename T >
void mydeque<T>::print_deque() const
{
    cout << "Sizeofmemory = " << _sizeofmemory << "; Sizeofdata = " << _sizeofdata << endl;
    for (int i = l; (i % _sizeofmemory) != r; ++i)
    {
        int ii =  (i % _sizeofmemory);
        cout << "d["<<ii<<"] = " << _data[ii] << " ";
    }
    cout << endl;
}

template< typename T >
T &mydeque<T>::operator[](int index)
{
    if (index >= _sizeofdata)
        assert("mydeque::operator[] : Error - index out of data");
    return _data[(index + l) % _sizeofmemory];
}

template< typename T >
void mydeque<T>::back(const T &element)
{
    if (_sizeofdata == 0)
        push_back(element);
    else
        _data[(l + 1) % _sizeofmemory] = element;
}

template< typename T >
const T mydeque<T>::back() const
{
    if (_sizeofdata == 0)
        assert("mydeque::back() : Error - deque is empty");
    return _data[(l + 1) % _sizeofmemory];
}

template< typename T >
void mydeque<T>::front(const T &element)
{
    if (_sizeofdata == 0)
        push_back(element);
    else
        _data[(r - 1 + _sizeofmemory) % _sizeofmemory] = element;
}

template< typename T >
const T mydeque<T>::front() const
{
    if (_sizeofdata == 0)
        assert("mydeque::front() : Error - deque is empty");
    return _data[(r - 1 + _sizeofmemory) % _sizeofmemory];
}

template< typename T >
bool mydeque<T>::empty() const
{
    return (_sizeofdata == 0);
}

template< typename T >
uint mydeque<T>::sizeofmemory() const
{
    return _sizeofmemory;
}



#endif // MYDEQUE
