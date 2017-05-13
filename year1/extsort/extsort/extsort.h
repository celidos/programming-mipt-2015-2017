#ifndef EXTSORT_H
#define EXTSORT_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "heapsort_mod.h"
#include "mergesort.h"

#define CType long long int

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::stringstream;

typedef unsigned int uint;

const uint MAX_BLOCK_SIZE = 3000000; // 50 MB; // 134217728 // = 2 ^ 27 el. = 512 MiB

class MyBlock
{
public:
    MyBlock(){};
    ~MyBlock();

    void Init(uint MyNumber, uint MaxSubPartSize);
    bool IsFinito();
    CType GetNextElement();

private:
    uint MAX_SUBBLOCK_SIZE;     // На куски каого размера можно разбивать блоки
    uint selfnumber;            // Номер блока
    uint indexinsubblock;       // итератор по подблоку
    uint sizeofsubblock;        // размер очередного подблока

    ifstream fin;               // Поток чтения из файла

    CType *subblock;            // буфер

    void operator=( const MyBlock& );

    void LoadNextPart();
};


struct mypair
{
    uint index;
    CType value;
    bool operator <(const mypair &b)
    {
        return this->value < b.value;
    }
    mypair(): index(0) {};

    mypair(uint block_index, CType Value)
    {
        index = block_index;
        value = Value;
    }
};

class KWayMergeHeap
{
public:
    KWayMergeHeap(uint Blockcount, const char* filename);
    ~KWayMergeHeap();

private:
    uint BLOCK_PARTITION_SIZE;
    uint blockcount;
    MyBlock *blocks;
    mypair *heap;
};

class ExtSort
{
public:
    ExtSort();
    ~ExtSort();

    void Sort(string Filename);
private:
    string filename;
    ifstream fin;

    uint blockcount;

    void InitInFileStream();
    void SortAndWriteBlock(CType *Array, uint size, int num);
    void DivideIntoParts();
    void MergeWithKWay();

};

#endif // EXTSORT_H

