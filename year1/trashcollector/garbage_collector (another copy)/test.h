#ifndef TEST_H
#define TEST_H

#include <garbage_collector.h>
#include <iostream>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::endl;

class MyClass: public SmartObject
{
public:
    int size;
    std::vector<SmartObject*> mypointers;
    virtual std::vector<SmartObject*> pointers() const {
        return mypointers;
    };

    ~MyClass() {}
};

void test1() // testMyClass2 должен быть удален
{
    printf("test1 ..................................................\n");

    MyClass testMyClass1;
    MyClass testSmart1;
    MyClass testSmart2;
    MyClass* testSmart3 = new MyClass;
    testMyClass1.size = 3;
    testMyClass1.mypointers.push_back(&testSmart1);
    testMyClass1.mypointers.push_back(&testSmart2);
    testMyClass1.mypointers.push_back(testSmart3);
    MyClass *testMyClass2 = new MyClass;
}

void test2()
{
    printf("test2 ..................................................\n");
    MyClass* testSmart1 = new MyClass;
    MyClass* testSmart2 = new MyClass;
    MyClass* testSmart3 = new MyClass;
    (*testSmart1).mypointers.push_back(testSmart2);
    (*testSmart2).mypointers.push_back(testSmart3);
    (*testSmart3).mypointers.push_back(testSmart1);
}

void test3()
{
    printf("test3 ..................................................\n");
    MyClass *lostVar = new MyClass;
    lostVar = new MyClass;
    lostVar = new MyClass;
    lostVar = new MyClass;

    MyClass anotherStackVar1;
    MyClass anotherStackVar2;
    MyClass anotherStackVar3;

    anotherStackVar1.mypointers.push_back(new MyClass);
    anotherStackVar1.mypointers.push_back(new MyClass);
    anotherStackVar1.mypointers.push_back(new MyClass);
    anotherStackVar1.mypointers.push_back(new MyClass);
    ((MyClass*)anotherStackVar1.mypointers[0])->mypointers.push_back(new MyClass);
    ((MyClass*)anotherStackVar1.mypointers[0])->mypointers.push_back(new MyClass);
    ((MyClass*)anotherStackVar1.mypointers[0])->mypointers.push_back(new MyClass);
    ((MyClass*)anotherStackVar1.mypointers[0])->mypointers.push_back(new MyClass);

    GarbageCollector::collect();
    lostVar = new MyClass;
    lostVar = new MyClass;
    GarbageCollector::collect();
    lostVar = new MyClass;
    lostVar = new MyClass;

    delete lostVar;
    lostVar = new MyClass;
    delete lostVar;
    lostVar = new MyClass;
    lostVar = new MyClass;
    lostVar = new MyClass;
    lostVar = new MyClass;
}

#endif // TEST_H

