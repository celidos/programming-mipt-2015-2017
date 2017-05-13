#include<iostream>
#include"merged_heaps.h"
//#include<gtest\gtest.h>
#include<ctime>
//#include<vld.h>

/*class HeapsTests : public ::testing::Test
{

};

TEST(HeapsTests, LeftHeapTest)
{
    MergedHeap<int> *a = new LeftHeap<int>();
    MergedHeap<int> *b = new LeftHeap<int>();
    for (int i = 100; i > 0; --i) {
        a->insert(i);
    }
    for (int i = 1; i < 101; ++i) {
        int tmp = a->extractMin();
        ASSERT_EQ(i, tmp);
    }
    for (int i = 100; i > 0; --i) {
        a->insert(i);
    }
    for (int i = 200; i > 100; --i) {
        b->insert(i);
    }
    a->merge(*b);
    for (int i = 1; i < 201; ++i) {
        int tmp = a->extractMin();
        ASSERT_EQ(i, tmp);
    }
    delete a;
    delete b;
}

TEST(HeapsTests, SkewHeapTest)
{
    MergedHeap<int> *a = new SkewHeap<int>();
    MergedHeap<int> *b = new SkewHeap<int>();
    for (int i = 100; i > 0; --i) {
        b->insert(i);
    }
    for (int i = 1; i < 101; ++i) {
        int tmp = b->extractMin();
        ASSERT_EQ(i, tmp);
    }
    for (int i = 100; i > 0; --i) {
        a->insert(i);
    }
    for (int i = 200; i > 100; --i) {
        b->insert(i);
    }
    a->merge(*b);
    for (int i = 1; i < 201; ++i) {
        int tmp = a->extractMin();
        ASSERT_EQ(i, tmp);
    }
    delete a;
    delete b;
}

TEST(HeapsTests, BinominalHeapTest)
{
    MergedHeap<int> *c = new BinominalHeap<int>();
    MergedHeap<int> *d = new BinominalHeap<int>();
    for (int i = 100; i > 0; --i) {
        c->insert(i);
    }
    for (int i = 1; i < 101; ++i) {
        int tmp = c->extractMin();
        ASSERT_EQ(i, tmp);
    }
    for (int i = 100; i > 0; --i) {
        c->insert(i);
    }
    for (int i = 200; i > 100; --i) {
        d->insert(i);
    }
    c->merge(*d);
    for (int i = 1; i < 201; ++i) {
        int tmp = c->extractMin();
        ASSERT_EQ(i, tmp);
    }
    delete c;
    delete d;
}*/

template<class T>
class test
{
public:
    test(int t1, int i1, int i2, T k)
    {
        type_ = t1;
        index1_ = i1;
        index2_ = i2;
        key_ = k;
    }
    int type_, index1_, index2_;
    T key_;
};

std::vector<test<int> > tests;

void genTest(int n)
{
    tests.clear();
    std::vector<int> heaps;
    int i = 0;
    while(i < n) {
        int tmp = rand() % 4;
        if (tmp == 0)
        {
            int key = rand();
            tests.push_back(test<int>(0, -1, -1, key));
            heaps.push_back(1);
            ++i;
        }
        if (tmp == 1 && heaps.size() != 0)
        {
            int index = rand() % heaps.size();
            int key = rand();
            tests.push_back(test<int>(1, index, -1, key));
            heaps[index]++;
            ++i;
        }
        if (tmp == 2 && heaps.size() != 0)
        {
            int index = rand() % heaps.size();
            if (heaps[index] > 0)
            {
                tests.push_back(test<int>(2, index, -1, -1));
                heaps[index]--;
                ++i;
            }
        }
        if (tmp == 3 && heaps.size() > 1)
        {
            int index1 = rand() % heaps.size();
            int index2 = rand() % heaps.size();
            if(index1 != index2)
            {
                tests.push_back(test<int>(3, index1, index2, -1));
                heaps[index1] += heaps[index2];
                heaps[index2] = heaps[heaps.size() - 1];
                heaps.pop_back();
                ++i;
            }
        }
    }
}

MergedHeap<int>* newHeap(int type, int key)
{
    if (type == 1)
        return new LeftHeap<int>(key);
    if (type == 2)
        return new SkewHeap<int>(key);
    return new BinominalHeap<int>(key);
}

clock_t testHeap(int n, int type)
{
    std::vector<MergedHeap<int>*> heaps;
    clock_t time = std::clock();
    for (int i = 0; i < n; ++i)
    {
        test<int> tmp = tests[i];
        if (tmp.type_ == 0)
        {
            std::cout << "NEW" << std::endl;
            heaps.push_back(newHeap(type, tmp.key_));
        }
        if (tmp.type_ == 1)
        {
            std::cout << "INS" << std::endl;
            heaps[tmp.index1_]->insert(tmp.key_);
        }
        if (tmp.type_ == 2)
        {
            std::cout << "EXT" << std::endl;
            heaps[tmp.index1_]->extractMin();
        }
        if (tmp.type_ == 3)
        {
            std::cout << "MERGE" << std::endl;
            heaps[tmp.index1_]->merge(*heaps[tmp.index2_]);
            std::cout << "DEL" << std::endl;
            delete heaps[tmp.index2_];
            heaps[tmp.index2_] = heaps[heaps.size() - 1];
            heaps.pop_back();
        }
    }
    time = std::clock() - time;
    for (size_t i = 0; i < heaps.size(); ++i) {
        std::cout << "DEL---" << std::endl;
        delete heaps[i];
    }
    return time;
}

int main(int argc, char** argv)
{
    //::testing::InitGoogleTest(&argc, argv);
    //int t = RUN_ALL_TESTS();
    srand(179);
    int n = 10;
    genTest(n);
    std::cout << "LeftHeap time " << testHeap(n, 1) << std::endl;
    //std::cout << "SkewHeap time " << testHeap(n, 2) << std::endl;
    //std::cout << "BinominalHeap time " << testHeap(n, 3) << std::endl;
    return 0;
}
