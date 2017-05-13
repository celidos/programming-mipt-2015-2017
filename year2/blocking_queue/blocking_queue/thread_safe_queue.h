#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

// AAAAAAAAASHOT TRUBA SHATAL

#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

template <class Value, class Container = std::deque<Value>>
class thread_safe_queue
{
public:
    explicit thread_safe_queue(size_t capacity);
    ~thread_safe_queue();

    void enqueue(Value v);
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
void thread_safe_queue<Value, Container>::enqueue(Value v)
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
            throw std::runtime_error("Error: thread_safe_queue::pop(..) called with empty queue");
        }

        cvOnPop_.wait(lock);

        if (isShuttedDown_.load())
        {
            cvOnPop_.notify_all();
            cvOnPush_.notify_all();
            throw std::runtime_error("Error: thread_safe_queue::pop(..) called with empty queue");
        }
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

#endif // BLOCKING_QUEUE_H

