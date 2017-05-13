#ifndef PARALLEL_SORT_H
#define PARALLEL_SORT_H

#include <iterator>
#include <algorithm>
#include <map>

#include <iostream>


#include "threadpool.h"

using std::endl;
using std::cout;
using std::cin;


const int MIN_DECOMPOSITION_LENGTH = 100;

class parallel_sort
{

public:
    parallel_sort(std::size_t nWorkers);

    template <class RandomAccessIterator, class Compare>
    void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

private:

    template <class RandomAccessIterator>
    struct mystruct
    {
        RandomAccessIterator first;
        RandomAccessIterator last;
        bool flag;

        //template <class RandomAccessIterator>
        mystruct() {}
        mystruct(RandomAccessIterator newfirst, RandomAccessIterator newlast, bool newflag = false)
        {
            first = newfirst;
            last = newlast;
            flag = newflag;
        }
    };

    std::size_t nThreads_;

    thread_pool<void> *pool_;

    template <class RandomAccessIterator, class Compare>
    static void sortSubpart(parallel_sort *psorter, std::map<size_t, mystruct<RandomAccessIterator> > &map, size_t vertex, Compare comp);

    template <class RandomAccessIterator, class Compare>
    static void merge(parallel_sort *psorter, std::map<size_t, mystruct<RandomAccessIterator> > &map, size_t vertex, Compare comp);

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
    std::map<size_t, mystruct<RandomAccessIterator> > map_;

    map_.insert(std::make_pair(static_cast<size_t>(0), mystruct<RandomAccessIterator>(first, last)));

    cout << "Going to sortSubpart ..." << endl;
    sortSubpart(this, map_, 0, comp);

    while (!map_.empty());
}

template <class RandomAccessIterator, class Compare>
void parallel_sort::merge(parallel_sort *psorter, std::map<size_t, mystruct<RandomAccessIterator> > &map, size_t vertex, Compare comp)
{
    cout << "Merge callew with " << endl;
    cout.flush();
    //void merge(T a[], long lb, long split, long ub)
    mystruct<RandomAccessIterator> mapElement, childleft, childright;

    {
        std::unique_lock<std::mutex> lock(psorter->mutex_);
        mapElement = map.at(vertex);
        childleft = map.at(vertex*2 + 1);
        childright = map.at(vertex*2 + 2);

        cout << "mergind gata: " << endl;
        for (auto it = childleft.first; it < childright.last; ++it)
        {
            cout << "[" << *it << "] ";
        }
        cout << endl;
    }
    cout << "(" << childleft.last - childleft.first << ") & (" << childright.last - childright.first << ")" << endl;

    RandomAccessIterator first1 = childleft.first;
    RandomAccessIterator last1  = childleft.last;
    RandomAccessIterator first2 = childright.first;
    RandomAccessIterator last2  = childright.last;

    typedef typename std::iterator_traits<RandomAccessIterator>::value_type T;
    T *tempBuffer = new T[static_cast<size_t>(last2 - first1)];

    RandomAccessIterator it1 = first1, it2 = first2;
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


    cout << "buffer : " << endl;
    bufferIndex = 0;
    for (auto it = first1; it < last2; ++it)
    {
        *it = tempBuffer[bufferIndex++];
        cout << *it << " ";
    }
    cout << " ### " << endl;

    delete [] tempBuffer;

    // --------------------------------

    {
        std::unique_lock<std::mutex> lock(psorter->mutex_);
        size_t parent = getparent(vertex);
        if (!map.at(parent).flag)
        {
            map.at(parent).flag = true;
        }
        else if (vertex)
        {
            psorter->pool_->submit(std::bind(merge<RandomAccessIterator, Compare>, psorter, std::ref(map), parent, comp));
        }
        else if (!vertex)
        {
            map.clear();
        }
    }

}

template <class RandomAccessIterator, class Compare>
void parallel_sort::sortSubpart(parallel_sort *psorter, std::map<size_t, mystruct<RandomAccessIterator> > &map, size_t vertex, Compare comp)
{

    mystruct<RandomAccessIterator> mapElement;

    {
        std::unique_lock<std::mutex> lock(psorter->mutex_);
        cout << "Sorting subpart. Vertex = (" << vertex;
        cout.flush();
        mapElement = map.at(vertex);
        cout << "), parent = <" << getparent(vertex) << ">; len = [" << map.at(vertex).last - map.at(vertex).first << "]"<< endl;
    }

    size_t parent = getparent(vertex);
    //cout << "), parent = <" << parent << ">; len = [" << map.at(vertex).last - map.at(vertex).first << "]"<< endl;
    if (static_cast<size_t>(mapElement.last - mapElement.first) > MIN_DECOMPOSITION_LENGTH)
    {
        cout << "v = " << vertex << "; HARD CASE - NON DECOMPOSITION LENGTH" << endl;
        RandomAccessIterator middle = mapElement.first + static_cast<size_t>(mapElement.last - mapElement.first) / 2;

        {
            std::unique_lock<std::mutex> lock(psorter->mutex_);

            map.insert(std::make_pair(static_cast<size_t>(vertex * 2 + 1), mystruct<RandomAccessIterator>(mapElement.first, middle)));
            map.insert(std::make_pair(static_cast<size_t>(vertex * 2 + 2), mystruct<RandomAccessIterator>(middle, mapElement.last)));
        }

        psorter->pool_->submit(std::bind(sortSubpart<RandomAccessIterator, Compare>, psorter, std::ref(map), vertex*2 + 1, comp));
        psorter->pool_->submit(std::bind(sortSubpart<RandomAccessIterator, Compare>, psorter, std::ref(map), vertex*2 + 2, comp));
    }
    else
    {
        cout << "DECOMPOSITION LENGTH" << endl;
        std::sort(mapElement.first, mapElement.last, comp);

        std::unique_lock<std::mutex> lock(psorter->mutex_);
        if (map.at(parent).flag)
        {
            cout << "Submitting merge of " << parent << endl;
            psorter->pool_->submit(std::bind(merge<RandomAccessIterator, Compare>, psorter, std::ref(map), parent, comp));
        }
        else
        {
            cout  << "PARENT " << parent << " FLAG = TRUE" << endl;
            map.at(parent).flag = true;
        }
    }
}

#endif // PARALLEL_SORT_H
