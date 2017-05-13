#ifndef ARRAY_TESTER_H
#define ARRAY_TESTER_H

#include <ctime>
#include <iostream>

#include <array_interface.h>
#include <naivearray.h>
#include <splaytree.h>

#define MAX_OPERATIONS 9
#define MAX_ABS 1000
#define MAX_ELEM 1500

class ArrayTester
{
public:
    IArray *implemA;
    IArray *implemB;

    ArrayTester() :
        implemA(nullptr), implemB(nullptr)
    {
        srand(time(0));
    }

    ArrayTester(IArray *_implem1, IArray *_implem2) :
        implemA(_implem1), implemB(_implem2)
    {
        srand(time(0));
    }

    void runTest(int operations, bool print);
};

void ArrayTester::runTest(int operations, bool print)
{
    srand( time(0) );

    int elementsCount = 1;
    int nextOp = 0;
    int operand1 = 0, operand2 = 0, operand3 = 0;

    bool isok = true;

    DATA newkey = rand() % 100;
    implemA->insert(0, newkey);
    implemB->insert(0, newkey);

    for (int ii = 0; ii < operations; ++ii)
    {
        if (print) {cout << "#" << ii << "  "; cout.flush();}

        operand1 = rand() % elementsCount;
        nextOp = rand() % MAX_OPERATIONS;

        if (nextOp == 0 && elementsCount > 5) // remove
        {
            if (print) {cout << "  ERS from " << operand1 << "... ";cout.flush();}

            implemA->remove(operand1);
            implemB->remove(operand1);

            --elementsCount;
        }
        else if (nextOp == 1) // add on range
        {
            operand2 = operand1 + rand() % (elementsCount - operand1);
            operand3 = rand() % MAX_ABS;

            if (print) {cout << "  ADD from " << operand1 << " to " << operand2 << "; add = " << operand3 << "... "; cout.flush();}

            implemA->add(operand1, operand2, operand3);
            implemB->add(operand1, operand2, operand3);
        }
        else if (nextOp == 2 && elementsCount > 0) // check one elem
        {
            if (print) {cout << "! CHK " << operand1 << "... "; cout.flush();}

            DATA el1 = implemA->getElem(operand1);
            DATA el2 = implemB->getElem(operand1);

            if (el1 != el2)
            {
                if (print) {cout << "Elements is not equal: e1 = " << el1 << "; e2 = " << el2 << endl; cout.flush();}
                isok = false;
            }
        }
        else if (nextOp == 3) // get sum
        {
            operand2 = operand1 + rand() % (elementsCount - operand1);

            if (print) {cout << "! SUM from " << operand1 << " to " << operand2 << "... "; cout.flush();}

            long long int sum1 = implemA->sum(operand1, operand2);
            long long int sum2 = implemB->sum(operand1, operand2);

            if (sum1 != sum2)
            {
                if (print) {cout << "Different summs: sum1 = " << sum1 << "; sum2 = " << sum2 << endl; cout.flush();}
                isok = false;
            }
        }
        else if (nextOp == 4) // reverse some range
        {
            operand2 = operand1 + rand() % (elementsCount - operand1);

            if (print) {cout << "  REV from " << operand1 << " to " << operand2 << "... "; cout.flush();}

            implemA->reverse(operand1, operand2);
            implemB->reverse(operand1, operand2);
        }
        else if (nextOp == 5) // next permutation
        {
            operand2 = operand1 + rand() % (elementsCount - operand1);

            if (print) {cout << "  NEXTP from " << operand1 << " to " << operand2 << "... "; cout.flush();}

            implemA->nextPermutation(operand1, operand2);
            implemB->nextPermutation(operand1, operand2);
        }
        else if (elementsCount < MAX_ELEM)// inserting
        {
            operand2 = (rand()*rand() + rand()) % MAX_ABS;

            if (print) {cout << "  INS '" << operand2 << "' at " << operand1 << "-th pos... "; cout.flush();}

            implemA->insert(operand1, operand2);
            implemB->insert(operand1, operand2);

            ++elementsCount;
        }
        else
        {
            if (print) {cout << "  SKP ..."; cout.flush(); }
        }
        if (print && isok) cout << "ok!" << endl;

        if (!isok)
        {
            cout << "FAILED!" << endl << endl << " ~~~ try to find a bug ~~~" << endl;
            cout << "Number of elements : " << elementsCount << endl;
            cout << "ImplemA elements:" << endl;
            implemA->print();
            cout << "ImplemB elements:" << endl;
            implemB->print();
            return;
        }
    }

    if (isok)
    {
        cout << endl << " _______________________ " << endl << "successfully completed! (" << operations << " ops)" << endl;
    }
}

#endif // ARRAY_TESTER_H
