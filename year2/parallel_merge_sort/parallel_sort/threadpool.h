#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

const unsigned int MAX_QUEUE_ELEMENTS = (1 << 30) - 1;

template <class Value, class Container = std::deque<Value>>
class thread_safe_queue
{
public:
    explicit thread_safe_queue(size_t capacity);
    ~thread_safe_queue();

    void enqueue(Value &&v);
    void pop(Value& v);
    void shutdown();

private:
    Container queue_;
    std::atomic_size_t capacity_;
    std::atomic_bool isShuttedDown_;

    std::mutex mutexQueue_;
    std::condition_variable cvOnPop_;
    std::condition_variable cvOnPush_;
};

template <class Value, class Container>
thread_safe_queue<Value, Container>::thread_safe_queue(size_t capacity)
{
    capacity_.store(capacity);
    isShuttedDown_.store(false);
}

template <class Value, class Container>
thread_safe_queue<Value, Container>::~thread_safe_queue()
{
    shutdown();
}

template <class Value, class Container>
void thread_safe_queue<Value, Container>::enqueue(Value &&v)
{
    std::unique_lock<std::mutex> lock(mutexQueue_);

    while (queue_.size() == capacity_.load())
        cvOnPush_.wait(lock);

    if (isShuttedDown_.load())
    {
        cvOnPop_.notify_all();
        cvOnPush_.notify_all();
        throw std::runtime_error("Error: thread_safe_queue::enqueue(..) called with closed queue");
    }

    queue_.push_back(std::move(v));
    cvOnPop_.notify_all();
}

template <class Value, class Container>
void thread_safe_queue<Value, Container>::pop(Value &v)
{
    std::unique_lock<std::mutex> lock(mutexQueue_);

    while (queue_.empty())
    {
        if (queue_.size() == 0 && isShuttedDown_.load())
        {
            cvOnPop_.notify_all();
            cvOnPush_.notify_all();
            throw std::logic_error("Error: thread_safe_queue::pop(..) called with empty queue");
        }
        cvOnPop_.wait(lock);
    }

    v = std::move(queue_.front());

    queue_.pop_front();
    cvOnPush_.notify_all();
}

template <class Value, class Container>
void thread_safe_queue<Value, Container>::shutdown()
{
    std::unique_lock<std::mutex> lock(mutexQueue_);
    isShuttedDown_.store(true);
    cvOnPop_.notify_all();
    cvOnPush_.notify_all();
}



// --------------------------------------------------------------------------



template <class Value>
class thread_pool
{
public:
    explicit thread_pool(size_t num_workers);
    ~thread_pool();

    std::future<Value> submit(std::function<Value()> task);
    void shutdown();

private:
    std::vector<std::thread> pool_;
    thread_safe_queue<std::packaged_task<Value()>> queue_;

    std::atomic_bool isShuttedDown_;
    std::atomic_bool isFinished_;
};

template <class Value>
inline thread_pool<Value>::thread_pool(size_t num_workers) :
    queue_(MAX_QUEUE_ELEMENTS)
{
    isShuttedDown_.store(false);
    isFinished_.store(false);

    for (size_t i = 0; i < num_workers; ++i)
    {
        pool_.emplace_back(
            [this]
            {
                while (true)
                {
                    std::packaged_task<Value()> task;

                    try
                    {
                        this->queue_.pop(task);
                    }
                    catch (std::logic_error)
                    {
                        return;
                    }

                    task();
                }
            }
        );
    }
}

template<class Value>
std::future<Value> thread_pool<Value>::submit(std::function<Value()> task)
{
    std::packaged_task<Value()> pckgTask = std::packaged_task<Value()>(task);
    std::future<Value> res = pckgTask.get_future();

    if (isShuttedDown_)
        throw std::runtime_error("Error: thread_pool::submit(..) called on stopped thread pool");

    queue_.enqueue(std::move(pckgTask));

    return res;
}

template <class Value>
void thread_pool<Value>::shutdown()
{
    isShuttedDown_.store(true);
    queue_.shutdown();

    for (std::thread &itThread : pool_)
        itThread.join();
}

template <class Value>
inline thread_pool<Value>::~thread_pool()
{
}

#endif // THREADPOOL_H
