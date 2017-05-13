#ifndef TICKET_SPINLOCK_H
#define TICKET_SPINLOCK_H

#include <atomic>
#include <thread>

class ticket_spinlock {
public:
    ticket_spinlock();
    void lock();
    void unlock();
private:
    std::atomic<std::size_t> freeticket;
    std::atomic<std::size_t> victim;
};

ticket_spinlock::ticket_spinlock()
{
    victim.store(0);
    freeticket.store(0);
}

void ticket_spinlock::lock()
{
    std::size_t curticket = freeticket.fetch_add(1);
    while (victim.load() != curticket)
    {
        std::this_thread::yield();
    }
}

void ticket_spinlock::unlock()
{
    victim.store(victim.load() + 1);
}

#endif // TICKET_SPINLOCK_H
