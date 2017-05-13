#include <naivearray.h>


DATA NaiveArray::getElem(size_t index)
{
    return array[index];
}

void NaiveArray::insert(size_t pos, DATA key)
{
    array.insert(array.begin() + pos, key);
}

void NaiveArray::change(size_t pos, DATA newkey)
{
    array[pos] = newkey;
}

void NaiveArray::remove(size_t pos)
{
    array.erase(array.begin() + pos);
}

void NaiveArray::reverse(size_t from, size_t to)
{
    std::reverse(array.begin() + from, array.begin() + to + 1);
}

void NaiveArray::add(size_t from, size_t to, DATA key)
{
    std::for_each(array.begin() + from, array.begin() + to + 1, [key] (DATA &x) {x += key;});
}

void NaiveArray::nextPermutation(size_t from, size_t to)
{
    std::next_permutation(array.begin() + from, array.begin() + to + 1);
}

void NaiveArray::print()
{
    std::cout << "[";
    std::for_each(array.begin(), array.end(), [] (DATA x) {std::cout << x << " ";});
    std::cout << "]" << std::endl;
}

DATA NaiveArray::sum(size_t from, size_t to)
{
    long long int sum = 0;
    std::for_each(array.begin() + from, array.begin() + to + 1, [&sum] (DATA x) {sum += x;});
    return sum;
}
