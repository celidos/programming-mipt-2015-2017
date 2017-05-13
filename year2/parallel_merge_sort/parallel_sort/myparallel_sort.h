#ifndef PARALLEL_SORT_H
#define PARALLEL_SORT_H

#include "threadpool.h"

#include <iterator>
#include <algorithm>
#include <map>

const int MIN_DECOMPOSITION_LENGTH = 100;

class parallel_sort
{
public:
    parallel_sort(std::size_t nWorkers);

    template <class raIterator, class Compare>
    void sort(raIterator first, raIterator last, Compare comp);

private:
    template <class raIterator>
    struct TreeNode
    {
        raIterator first;
        raIterator last;
        std::atomic_bool flag;

        TreeNode() {}
        TreeNode(raIterator newfirst, raIterator newlast, bool newflag = false)
        {
            first = newfirst;
            last = newlast;
            flag = newflag;
        }

        TreeNode(const TreeNode &ms) :
            first(ms.first), last(ms.last) {
            flag.store(ms.flag.load());
        }

        TreeNode &operator=(const TreeNode &ms) {
            first = ms.first;
            last = ms.last;
            flag.store(ms.flag.load());
            return *this;
        }
    };

    thread_pool<void> *pool_;

    template <class raIterator, class Compare>
    static void sortSubpart(parallel_sort *psorter, std::map<size_t, TreeNode<raIterator> > &map, size_t vertex, Compare comp);

    template <class raIterator, class Compare>
    static void merge(parallel_sort *psorter, std::map<size_t, TreeNode<raIterator> > &map, size_t vertex, Compare comp);

    std::mutex mutex_;
};

// -------------------------------------------------------

parallel_sort::parallel_sort(std::size_t nWorkers)
{
    pool_ = new thread_pool<void>(nWorkers);
}

size_t getparent(size_t vertex)
{
    return static_cast<size_t>(((long int)vertex - 1) / 2);
}

template <class RandomAccessIterator, class Compare>
void parallel_sort::sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    std::map<size_t, TreeNode<RandomAccessIterator> > map_;

    map_.emplace(static_cast<size_t>(0), TreeNode<RandomAccessIterator>(first, last));

    sortSubpart(this, map_, 0, comp);

    while (true)
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
            if (map_.empty())
                break;
        lock.unlock();

        std::this_thread::yield();
    };
}

template <class raIterator, class Compare>
void parallel_sort::merge(parallel_sort *psorter, std::map<size_t, TreeNode<raIterator> > &map, size_t vertex, Compare comp)
{
    TreeNode<raIterator> mapElement, childleft, childright;

    {
        std::unique_lock<std::mutex> lock(psorter->mutex_);
        mapElement = map.at(vertex);
        childleft = map.at(vertex*2 + 1);
        childright = map.at(vertex*2 + 2);
    }

    raIterator first1 = childleft.first;
    raIterator last1  = childleft.last;
    raIterator first2 = childright.first;
    raIterator last2  = childright.last;

    typedef typename std::iterator_traits<raIterator>::value_type T;
    T *tempBuffer = new T[static_cast<size_t>(last2 - first1)];

    raIterator it1 = first1, it2 = first2;
    size_t bufferIndex = 0;

    while (it1 < last1 && it2 < last2)
    {
        if (!comp(*it1, *it2))
        {
            tempBuffer[bufferIndex] = *it2;
            ++bufferIndex;
            ++it2;
        }
        else
        {
            tempBuffer[bufferIndex] = *it1;
            ++bufferIndex;
            ++it1;
        }
    }

    while (it2 < last2)
    {
        tempBuffer[bufferIndex] = *it2;
        ++bufferIndex;
        ++it2;
    }
    while (it1 < last1)
    {
        tempBuffer[bufferIndex] = *it1;
        ++bufferIndex;
        ++it1;
    }

    bufferIndex = 0;
    for (auto it = first1; it < last2; ++it)
        *it = tempBuffer[bufferIndex++];

    delete [] tempBuffer;

    // --------------------------------

    {
        std::unique_lock<std::mutex> lock(psorter->mutex_);
        size_t parent = getparent(vertex);
        if (!map.at(parent).flag)
            map.at(parent).flag = true;
        else if (vertex)
            psorter->pool_->submit(std::bind(merge<raIterator, Compare>, psorter, std::ref(map), parent, comp));
        else if (!vertex)
            map.clear();
    }
}

template <class raIterator, class Compare>
void parallel_sort::sortSubpart(parallel_sort *psorter, std::map<size_t, TreeNode<raIterator> > &map, size_t vertex, Compare comp)
{
    TreeNode<raIterator> mapElement;

    {
        std::unique_lock<std::mutex> lock(psorter->mutex_);
        mapElement = map.at(vertex);
    }

    size_t parent = getparent(vertex);

    if (static_cast<size_t>(mapElement.last - mapElement.first) > MIN_DECOMPOSITION_LENGTH)
    {
        raIterator middle = mapElement.first + static_cast<size_t>(mapElement.last - mapElement.first) / 2;
        {
            std::unique_lock<std::mutex> lock(psorter->mutex_);
            map.insert(std::make_pair(static_cast<size_t>(vertex * 2 + 1), TreeNode<raIterator>(mapElement.first, middle)));
            map.insert(std::make_pair(static_cast<size_t>(vertex * 2 + 2), TreeNode<raIterator>(middle, mapElement.last)));
        }
        psorter->pool_->submit(std::bind(sortSubpart<raIterator, Compare>, psorter, std::ref(map), vertex*2 + 1, comp));
        psorter->pool_->submit(std::bind(sortSubpart<raIterator, Compare>, psorter, std::ref(map), vertex*2 + 2, comp));
    }
    else
    {
        std::sort(mapElement.first, mapElement.last, comp);

        std::unique_lock<std::mutex> lock(psorter->mutex_);
        if (vertex)
        {
            if (map.at(parent).flag)
                psorter->pool_->submit(std::bind(merge<raIterator, Compare>, psorter, std::ref(map), parent, comp));
            else
                map.at(parent).flag = true;
        }
        else
            map.clear();
    }
}

#endif // PARALLEL_SORT_H
