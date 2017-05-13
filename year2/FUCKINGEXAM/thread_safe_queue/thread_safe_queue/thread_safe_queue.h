#include <atomic>
#include <thread>
#include <mutex>

class semaphore;

template <class Value, class Container = std::deque<Value>>
class thread_safe_queue {
public:
    explicit thread_safe_queue(size_t capacity);
    void enqueue(Value v);
    void pop(Value& v);
    void shutdown();
};
