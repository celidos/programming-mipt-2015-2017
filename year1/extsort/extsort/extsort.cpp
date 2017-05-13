#include <extsort.h>

ExtSort::ExtSort() :
    blockcount(0)
{

}

ExtSort::~ExtSort()
{
    fin.close();
}

void ExtSort::InitInFileStream()
{
    cout << "Init Input File stream ...";
    fin.open(filename.c_str());
    cout << " ok!" << endl;
}

void ExtSort::SortAndWriteBlock(CType *Array, uint size, int num)
{
    stringstream ss;
    ss << "_extsort_BLOCK" << num << ".exs";
    ofstream fout(ss.str().c_str());

    _internal_merge_sort(Array, 0, size);

    for (uint i = 0; i < size; ++i)
        fout << Array[i] << endl;
}

void ExtSort::DivideIntoParts()
{
    cout << "Started Dividing into parts ..." << endl;
    CType *buffer = new CType [MAX_BLOCK_SIZE];

    uint iter = 0;
    while (fin >> buffer[iter])
    {
        ++iter;

        if (iter >= MAX_BLOCK_SIZE)         // прочитали блок и запишем его
        {
            cout << "Block # " << (blockcount) << "; 1-st element : " << buffer[0] << endl;
            SortAndWriteBlock(buffer, MAX_BLOCK_SIZE, blockcount++);
            iter = 0;
        }
    }

    if (iter > 0)                           // мог остаться кусок длины меньше MAX_BLOCK_SIZE
    {
        cout << "Block # " << (blockcount) << "; 1-st element : " << buffer[iter] << endl;
        SortAndWriteBlock(buffer, iter, blockcount++);
    }

    delete [] buffer;
    cout << " ok!" << endl;
}

void ExtSort::MergeWithKWay()
{
    KWayMergeHeap(blockcount, filename.c_str());
}

void ExtSort::Sort(string Filename)
{
    filename = Filename;

    InitInFileStream();

    DivideIntoParts();

    MergeWithKWay();

    cout << endl << " ~~ COMPLETED ~~ "<< endl;
}

KWayMergeHeap::KWayMergeHeap(uint Blockcount, const char *filename)
{
    cout << "Start merging ..." << endl;
    cout << " * Initialization and building heap ..." << endl;
    blockcount = Blockcount;
    BLOCK_PARTITION_SIZE = MAX_BLOCK_SIZE / blockcount;
    cout << "BLOCK_PARTITION_SIZE = " << BLOCK_PARTITION_SIZE << endl;

    // Инициализируем блоки и строим начальную кучу
    blocks = new MyBlock[blockcount];
    heap   = new mypair [blockcount];

    for (uint i = 0; i < blockcount; ++i)
    {
        blocks[i].Init(i, BLOCK_PARTITION_SIZE);
        heap[i].value = blocks[i].GetNextElement();
        heap[i].index = i;
    }

    HeapBuild(heap, blockcount);
    cout << " ok!" <<endl;
    // Инициализируем файловый поток вывода

    ofstream fout("result.txt");

    // Все инициализировано. Начинаем K-путевое слияние

    int forcout = 0;

    int heapsize = blockcount;
    while (heapsize > 0)
    {
        fout << heap[0].value << endl;

        int curBlock = heap[0].index;
        if (!blocks[curBlock].IsFinito())
        {
            heap[0].value = blocks[curBlock].GetNextElement();
            ++forcout;
            if (forcout >= MAX_BLOCK_SIZE)
            {
                forcout = 0;
                cout << ". Element # " << heap[0].value << endl;
            }
            HeapSiftDown(heap, heapsize, 0);
        }
        else
        {
            ExtractMin(heap, heapsize);
            --heapsize;
        }
    }

    fout.close();
}

KWayMergeHeap::~KWayMergeHeap()
{
    if (blocks)
        delete [] blocks;

    if (heap)
        delete [] heap;
}

MyBlock::~MyBlock()
{
    if (subblock != NULL)
        delete [] subblock;
}

void MyBlock::Init(uint MyNumber, uint MaxSubPartSize)
{
    selfnumber = MyNumber;
    MAX_SUBBLOCK_SIZE = MaxSubPartSize;
    stringstream filename;
    filename << "_extsort_BLOCK" << selfnumber << ".exs";
    fin.open(filename.str().c_str());

    subblock = new CType [MAX_SUBBLOCK_SIZE];

    LoadNextPart();
}

bool MyBlock::IsFinito()
{
    return (sizeofsubblock <= 0);
}

CType MyBlock::GetNextElement()
{
    CType x = subblock[indexinsubblock++];

    if (indexinsubblock >= sizeofsubblock)
        LoadNextPart();

    return x;
}

void MyBlock::LoadNextPart()
{
    uint index = 0;
    while (fin >> subblock[index])
    {
        ++index;

        if (index >= MAX_SUBBLOCK_SIZE)
            break;
    }

    sizeofsubblock = index;
    indexinsubblock = 0;
}
