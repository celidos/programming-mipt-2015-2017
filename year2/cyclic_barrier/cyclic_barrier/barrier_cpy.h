#ifndef BARRIER_H
#define BARRIER_H

#include <atomic>
#include <mutex>
#include <array>
#include <thread>
#include <condition_variable>
#include <iostream>

using std::size_t;
using std::condition_variable;
using std::mutex;
using std::unique_lock;
using std::atomic;

using std::cin;
using std::cout;
using std::endl;

using std::thread;

class barrier
{
public:
    explicit barrier(size_t num_threads);
    void enter();
private:
    mutex mtx_in;
    mutex mtx_out;

    //atomic<int> outDoor;
    atomic<int> counter;
    atomic<bool> flag;
    size_t max_threads;


    condition_variable cv_out;
    condition_variable cv_in;

    void clear_counter();
};

barrier::barrier(size_t num_threads) :
    max_threads(num_threads)
{
    clear_counter();
    flag.store(1);
}

void barrier::enter()
{
    // -------------------------------------------------

    unique_lock<mutex> lock_in(mtx_in);
    while (!flag.load() && counter.load() > 0)
    {
        cv_in.wait(lock_in);
    }

    // --------------------------------------------------

    ++counter;

    if (counter.load() >= (int)max_threads)
    {
        flag.store(false);
        cv_out.notify_all();
    }
    else
    {
        while (!flag.load())
        {
            cv_out.wait(lock_in);
        }
    }

    --counter;

    if (counter.load() == 0)
    {
        flag.store(true);
        cv_in.notify_all();
    }
    lock_in.unlock();
}

void barrier::clear_counter()
{
    counter.store(0);
}

#endif
