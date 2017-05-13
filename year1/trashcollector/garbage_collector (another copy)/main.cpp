#include <iostream>

#include <test.h>

int main()
{
    test1();
    test2();
    test3();

    GarbageCollector::DeleteInstance();
}
