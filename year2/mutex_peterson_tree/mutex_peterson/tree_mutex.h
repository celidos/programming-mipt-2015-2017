#ifndef TREE_MUTEX
#define TREE_MUTEX

#include <vector>
#include <atomic>
#include <array>
#include <thread>

using std::vector;
using std::array;

// --------------------------------------------------------------

class PetersonMutex
{
public:
    PetersonMutex()
    {
        want[0].store(false);
        want[1].store(false);
        waiting.store(0);
    }

    void lock(int threadId)
    {
        int other = 1 - threadId;
        want[threadId].store(true);
        waiting.store(threadId);
        while (want[other].load() && waiting.load() == threadId) {
            std::this_thread::yield();
        }
        owner = threadId;
    }

    void unlock(int threadId) {
        want[threadId].store(false);
    }
    std::atomic<int> owner;
private:
    std::array<std::atomic<bool>, 2> want;
    std::atomic<int> waiting;

};

// ---------------------------------------------------------------

class tree_mutex
{
public:
    tree_mutex(size_t num_threads);
    ~tree_mutex();

    void lock(size_t thread_index);
    void unlock(size_t thread_index);
private:
    size_t _numThreads;
    size_t _treeSize;
    vector<PetersonMutex*> tree;
};

int getMeqPowerOf2of(int x)
{
    int ans = 1;
    while (ans < static_cast<int>(x)) ans <<= 1;
    return ans;
}

tree_mutex::tree_mutex(size_t num_threads) :
    _numThreads(num_threads)
{
    _treeSize = getMeqPowerOf2of(_numThreads) - 1;
    if (_treeSize == 0) _treeSize = 1;

    for (size_t i = 0; i < _treeSize; ++i)
        tree.push_back(new PetersonMutex);
}

tree_mutex::~tree_mutex()
{
    for (size_t i = 0; i < _treeSize; ++i)
        delete tree[i];
}

void tree_mutex::unlock(size_t thread_index)
{
    int aim = _treeSize + thread_index;
    int it = 0;
    do {
        int next = it * 2 + tree[it]->owner + 1;
        tree[it]->unlock(tree[it]->owner);
        it = next;
    } while (it < aim);
}

void tree_mutex::lock(size_t thread_index)
{
    int parity = (thread_index % 2);
    int it = _treeSize + thread_index;
    do {
        parity = 1 - (it % 2);
        it = (it - 1) / 2;
        tree[it]->lock(parity);
    } while (it > 0);
}

#endif
