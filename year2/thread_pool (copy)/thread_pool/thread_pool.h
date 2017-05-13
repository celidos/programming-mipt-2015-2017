#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

template <class Value>
class thread_pool
{
public:
    explicit thread_pool(size_t num_workers);
    ~thread_pool();

    std::future<Value> submit(std::function<Value()> task);
    void shutdown();

private:
    std::vector< std::thread > workers;
    std::queue< std::function<void()> > tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

template <class Value>
inline thread_pool<Value>::thread_pool(size_t num_workers)
    :   stop(false)
{
    for (size_t i = 0; i < num_workers; ++i)
    {
        workers.emplace_back(
            [this]
            {
                while (true)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);

                        this->condition.wait(lock,
                            [this]{
                                return this->stop || !this->tasks.empty();
                            }
                        );

                        if (this->stop && this->tasks.empty())
                            return;

                        task = std::move(this->tasks.front());
                        this->tasks.pop();
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
    std::future<Value> res = task->get_future();

    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }

    condition.notify_one();

    return res;
}

template <class Value>
inline thread_pool<Value>::~thread_pool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    condition.notify_all();

    for (std::thread &itWorker : workers)
        itWorker.join();
}


#endif // THREAD_POOL_H
