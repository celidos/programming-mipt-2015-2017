#include <memory>
#include <cstdlib>
#include <mutex>
#include <vector>
#include <list>
#include <thread>
#include <algorithm>
#include <set>
#include <cassert>
#include <atomic>

//----------------------------------------------------------
#include <sched.h>
//----------------------------------------------------------

using namespace std;

static const size_t SUPERBLOCK_SIZE = 8 * 1024; // 8 Kb
static const size_t EMPTY_FRACTION = 10;
static const size_t FREE_SUPERBLOCKS = 10;

class MultithreadAllocator // !
{
    typedef char byte; // !

    struct blockInfo {
        void* superblockOwner;
    };

    static const size_t BLOCK_OFFSET = sizeof(blockInfo);
    static const size_t GLOBAL_HEAP_ID = numeric_limits<size_t>::max();

    struct superblock {
        byte *ptr;
        size_t usesMemory;
        size_t heapOwner;
        size_t blockSize;

        list <size_t> freeBlocks;

        list<superblock*>::iterator fullnessGroupIt;

        superblock(size_t block_size) {
            ptr = nullptr;
            usesMemory = 0;
            heapOwner = 0;
            blockSize = block_size;
            for (size_t i = blockSize; i <= SUPERBLOCK_SIZE; i += blockSize) {
                freeBlocks.push_back(i - blockSize);
            }
        }

        void *getBlock() {
            if (freeBlocks.empty()) {
                return nullptr;
            }
            auto pt = freeBlocks.front();
            freeBlocks.pop_front();
            return ptr + pt;
        }

        void deallocate(void *blockPtr) {
            byte *bytePtr = reinterpret_cast<byte *>(blockPtr);
            size_t pos = bytePtr - ptr;
            freeBlocks.push_front(pos);
        }

        ~superblock() {
            free(ptr);
        }
    };

// -----------------------------------------------------

    struct bin {
        const static size_t FULNESS_GROUP_COUNT = 2;
        size_t FULNESS_GROUP_DELTA;

        //how much free blocks
        std::list<superblock *> fullnessGroups[FULNESS_GROUP_COUNT];
        list<superblock *> fullSuperblocks;

        size_t allocatedMemory;
        size_t usesMemory;

        bin(size_t blockSize) {
            static_assert(SUPERBLOCK_SIZE % FULNESS_GROUP_COUNT == 0, "SUPERBLOCK_SIZE % FULNESS_GROUP_COUNT");

            allocatedMemory = 0;
            usesMemory = 0;

            FULNESS_GROUP_DELTA = SUPERBLOCK_SIZE / blockSize / FULNESS_GROUP_COUNT;
        }

        void addSuperblock(superblock *spblock) {
            if (spblock->freeBlocks.empty()) {
                fullSuperblocks.push_front(spblock);
                spblock->fullnessGroupIt = fullSuperblocks.begin();
                return;
            }
            size_t curGroup = spblock->freeBlocks.size() / FULNESS_GROUP_DELTA;

            if (curGroup == FULNESS_GROUP_COUNT) {
                curGroup--;
            }

            fullnessGroups[curGroup].push_front(spblock);
            spblock->fullnessGroupIt = fullnessGroups[curGroup].begin();
        }

        superblock* getEmptiest() {
            for (size_t j = 0; j < FULNESS_GROUP_COUNT; j++) {
                size_t i = FULNESS_GROUP_COUNT - j - 1;
                if (fullnessGroups[i].empty()) {
                    continue;
                }
                superblock* spblock = fullnessGroups[i].front();
                fullnessGroups[i].pop_front();
                return spblock;
            }
            return nullptr;
        }

        void deallocate(superblock* spblock, void* ptr) {
            if (spblock->freeBlocks.empty()) {
                spblock->deallocate(ptr);
                fullSuperblocks.erase(spblock->fullnessGroupIt);
                fullnessGroups[FULNESS_GROUP_COUNT - 1].push_front(spblock);
                spblock->fullnessGroupIt = fullnessGroups[FULNESS_GROUP_COUNT - 1].begin();
                return;
            }

            size_t oldGroup = spblock->freeBlocks.size() / FULNESS_GROUP_DELTA;
            spblock->deallocate(ptr);
            size_t newGroup = spblock->freeBlocks.size() / FULNESS_GROUP_DELTA;
            if (newGroup == FULNESS_GROUP_COUNT) {
                newGroup--;
            }

            assert(oldGroup < FULNESS_GROUP_COUNT);
            assert(newGroup < FULNESS_GROUP_COUNT);

            if (oldGroup != newGroup) {
                fullnessGroups[oldGroup].erase(spblock->fullnessGroupIt);
                fullnessGroups[newGroup].push_front(spblock);
                spblock->fullnessGroupIt = fullnessGroups[newGroup].begin();
            }
        }

        pair<superblock *, void *> scan() {
            for (size_t i = 0; i < FULNESS_GROUP_COUNT; i++) {
                if (fullnessGroups[i].empty()) {
                    continue;
                }
                superblock *spblock = fullnessGroups[i].front();
                fullnessGroups[i].pop_front();
                return {spblock, spblock->getBlock()};
            }
            return {nullptr, nullptr};
        }

        ~bin() {
            for (auto &elem : fullSuperblocks) {
                delete elem;
            }
            for (auto &group : fullnessGroups) {
                for (auto &elem : group) {
                    delete elem;
                }
            }

        }
    };

    struct heap {
        const size_t GROW_FACTOR = 2;
        const size_t INITIAL_SIZE = 8;

        mutex heapMutex;

        vector<bin> bins;

        heap() {
            size_t curSize = INITIAL_SIZE;
            while (curSize <= SUPERBLOCK_SIZE / 2) {
                bins.push_back(bin(curSize));
                curSize *= GROW_FACTOR;
            }
        }

        size_t getSizeClass(size_t size) {
            size_t curSize = INITIAL_SIZE;
            size_t curBin = 0;
            while (curSize < size) {
                curSize *= GROW_FACTOR;
                curBin++;
            }
            return curBin;
        }

        size_t getBlockSize(size_t size) {
            size_t curSize = INITIAL_SIZE;
            while (curSize < size) {
                curSize *= GROW_FACTOR;
            }
            return curSize;
        }

        bin& findBin(size_t size) {
            size_t curBin = getSizeClass(size);
            return bins[curBin];
        }

        ~heap() {}
    };

    MultithreadAllocator() {
        heapsCount = thread::hardware_concurrency() * 2;
        heaps = new heap*[heapsCount];
        for (size_t i = 0; i < heapsCount; i++) {
            heaps[i] = new heap();
        }
        globalHeap = new heap();
    }

    size_t heapsCount;
    heap **heaps;
    heap *globalHeap;

public:

    void *allocate(size_t bytes) {

        size_t totalBytes = bytes + BLOCK_OFFSET;
        void *ptr = nullptr;

        //large superblock
        if (bytes > SUPERBLOCK_SIZE / 2) {
            ptr = malloc(totalBytes);
            blockInfo *info = reinterpret_cast<blockInfo *>(ptr);
            info->superblockOwner = nullptr;
            byte *bytePtr = reinterpret_cast<byte *>(ptr);
            return bytePtr + BLOCK_OFFSET;
        }


        auto heapId = std::hash<std::thread::id>()(std::this_thread::get_id()) % heapsCount;

        heap *curHeap = heaps[heapId];

        unique_lock<mutex> lock(curHeap->heapMutex);

        bin& curHeapBin = curHeap->findBin(totalBytes);

        pair<superblock *, void *> allocatedBlock = curHeapBin.scan();
        superblock *spblock = allocatedBlock.first;

        ptr = allocatedBlock.second;

        //unique_lock<mutex> superblockLock;

        if (spblock == nullptr) {
            unique_lock<mutex> globalHeapLock(globalHeap->heapMutex);

            bin& globalHeapBin = globalHeap->findBin(totalBytes);

            pair<superblock *, void *> heapAllocatedBlock = globalHeapBin.scan();
            superblock *globalHeapSuperblock = heapAllocatedBlock.first;

            if (globalHeapSuperblock != nullptr) {

                //superblockLock = unique_lock<mutex>(heapAllocatedBlock.first->superblockMutex);

                ptr = heapAllocatedBlock.second;
                spblock = globalHeapSuperblock;
                spblock->heapOwner = heapId;

                globalHeapBin.usesMemory -= spblock->usesMemory;
                curHeapBin.usesMemory += spblock->usesMemory;

                globalHeapBin.allocatedMemory -= SUPERBLOCK_SIZE;
                curHeapBin.allocatedMemory += SUPERBLOCK_SIZE;

            } else {

                spblock = new superblock(globalHeap->getBlockSize(totalBytes));
                spblock->ptr = (byte *) malloc(SUPERBLOCK_SIZE);
                spblock->heapOwner = heapId;
                ptr = spblock->getBlock();

                curHeapBin.allocatedMemory += SUPERBLOCK_SIZE;
            }
        } else {
            //superblockLock = unique_lock<mutex>(spblock->superblockMutex);
        }

        byte *bytePtr = reinterpret_cast<byte *>(ptr);

        blockInfo *info = reinterpret_cast<blockInfo *>(bytePtr);


        info->superblockOwner = spblock;

        //info->superblockOwner.store(spblock, memory_order_release);

        spblock->usesMemory += spblock->blockSize;
        curHeapBin.usesMemory += spblock->blockSize;

        curHeapBin.addSuperblock(spblock);


        ptr = bytePtr + BLOCK_OFFSET;
        return ptr;
    }

    void deallocate(void *ptr) {
        byte *bytePtr = reinterpret_cast<byte *>(ptr) - BLOCK_OFFSET;

        blockInfo *info = reinterpret_cast<blockInfo *>(bytePtr);

        //superblock *spblock = reinterpret_cast<superblock *>(info->superblockOwner.load(memory_order_acquire));
        superblock *spblock = reinterpret_cast<superblock *>(info->superblockOwner);

        if (spblock == nullptr) {
            free(bytePtr);
            return;
        }



        size_t curHeapOwner;
        heap* curHeap;
        unique_lock<mutex> heapLock;



        do {
            curHeapOwner = spblock->heapOwner;
            if (curHeapOwner == GLOBAL_HEAP_ID) {
                heapLock = unique_lock<mutex>(globalHeap->heapMutex);
                curHeap = globalHeap;
            } else {
                heapLock = unique_lock<mutex>(heaps[curHeapOwner]->heapMutex);
                curHeap = heaps[curHeapOwner];
            }
        } while (spblock->heapOwner != curHeapOwner);

        bin& curHeapBin = curHeap->findBin(spblock->blockSize);

        spblock->usesMemory -= spblock->blockSize;
        curHeapBin.usesMemory -= spblock->blockSize;

        curHeapBin.deallocate(spblock, bytePtr);

        if (curHeapOwner == GLOBAL_HEAP_ID) {
            return;
        }

        if ((curHeapBin.usesMemory < curHeapBin.allocatedMemory - FREE_SUPERBLOCKS * SUPERBLOCK_SIZE) &&
            (curHeapBin.usesMemory * EMPTY_FRACTION < (EMPTY_FRACTION - 1) * curHeapBin.allocatedMemory)) {

            unique_lock<mutex> globalHeapLock(globalHeap->heapMutex);
            bin& globalHeapBin = globalHeap->findBin(spblock->blockSize);

            superblock *emptiestBlock = curHeapBin.getEmptiest();

            assert(emptiestBlock != nullptr);

            //unique_lock<mutex> emptiestBlockLock(emptiestBlock->superblockMutex);

            emptiestBlock->heapOwner = GLOBAL_HEAP_ID;

            globalHeapBin.usesMemory += emptiestBlock->usesMemory;
            curHeapBin.usesMemory -= emptiestBlock->usesMemory;

            globalHeapBin.allocatedMemory += SUPERBLOCK_SIZE;
            curHeapBin.allocatedMemory -= SUPERBLOCK_SIZE;

            globalHeapBin.addSuperblock(emptiestBlock);

        }
    }

    static MultithreadAllocator &getInstance() {
        static MultithreadAllocator instance;
        return instance;
    }

    ~MultithreadAllocator() {
        for (size_t i = 0; i < heapsCount; i++) {
            delete heaps[i];
        }
        delete[] heaps;
        delete globalHeap;
    }
};


extern void *mtalloc(size_t bytes) {
    return MultithreadAllocator::getInstance().allocate(bytes);
}

extern void mtfree(void *ptr) {
    MultithreadAllocator::getInstance().deallocate(ptr);
}
