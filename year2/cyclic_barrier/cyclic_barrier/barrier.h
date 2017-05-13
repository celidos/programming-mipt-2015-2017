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
    mutex mtxIn;

    //atomic<int> outDoor;
    std::atomic_int counter;
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
    flag.store(true);
}

void barrier::enter()
{
    // -------------------------------------------------

    unique_lock<mutex> lockin(mtxIn);
    while (!flag.load())
    {
        //cout << (std::this_thread::get_id()) << " : " << "ololo waitin" << endl;
        cv_in.wait(lockin);
    }
    lockin.unlock();

    // --------------------------------------------------
unique_lock<mutex> lockout(mtxIn);

    //cout << (std::this_thread::get_id()) << " : " <<"counter == " << counter.load() << endl;
    counter.fetch_add(1);

    if (counter.load() >= (int)max_threads)
    {
        //clear_counter();
        flag.store(false);
        cv_out.notify_all();
    }
    else
    {
        // cout <<(std::this_thread::get_id()) << " : " << "case2 ...... " << endl;
        while (flag.load())
        {
            cv_out.wait(lockout);
        }
    }
    //cout << (std::this_thread::get_id()) << " : " <<"PO = " << counter.load() << endl;
    counter.fetch_add(-1);

    if (counter.load() == 0)
    {
        flag.store(true);
        cv_in.notify_all();
    }
    lockout.unlock();
}

void barrier::clear_counter()
{
    counter.store(0);
}

#endif
