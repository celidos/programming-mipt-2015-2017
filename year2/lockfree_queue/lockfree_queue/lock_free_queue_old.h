#ifndef LOCKFREE_QUEUE_H
#define LOCKFREE_QUEUE_H

#include <iostream>
#include <thread>
#include <atomic>
#include <deque>
#include <condition_variable>
#include <mutex>

#define CAS(x, y, z) x.compare_exchange_strong((y), (z))

using std::cin;
using std::cout;
using std::endl;
using std::atomic;

template<typename T>
class lock_free_queue
{
public:
    lock_free_queue();
    ~lock_free_queue();

    void enqueue(T v);
    bool dequeue(T &ans);

private:
    struct Node
    {
        Node()             { aim = nullptr; }
        Node(T newvalue)   { value = newvalue; aim = nullptr; }
        Node(T &&newvalue) { value = newvalue; aim = nullptr; }

        atomic<Node*> aim;
        T value;
    };

    atomic<Node*> head;
    atomic<Node*> tail;
    atomic<Node*> start;
};

template<typename T>
lock_free_queue<T>::lock_free_queue()
{
    head.store(new Node());
    tail.store(head);
    start.store(head);
}

template<typename T>
void lock_free_queue<T>::enqueue(T v)
{
    Node *newnode = new Node(v);
    Node *curTail;
    Node *tailAim;

    //cout << "thread_id = " << std::this_thread::get_id() << "ENQUEUE" << endl;

    while (true)
    {
        curTail = tail.load();
        tailAim = curTail->aim.load();

        if (!tailAim)
        {
            if (tail.load()->CAS(aim, tailAim, newnode))
                break;
        }
        else
        {
            //cout << "thread_id = " << std::this_thread::get_id() << "ENQUEUE CAS TAIL TAILAIM" << endl;
            CAS(tail, curTail, tailAim);
        }
    }
    //cout << "thread_id = " << std::this_thread::get_id() << "ENQUEUE last CAS" << endl;
    CAS(tail, curTail, newnode);
}

template<typename T>
bool lock_free_queue<T>::dequeue(T &ans)
{
    Node *curHead;
    Node *curTail;
    Node *headAim;

    //cout << "thread_id = " << std::this_thread::get_id() << "DEQUEUE entered" << endl;

    while (true)
    {
        curHead = head.load();
        curTail = tail.load();
        headAim = curHead->aim.load();

        if (curHead == curTail)
        {
            if (!headAim)
                return false;
            else
            {
                //cout << "thread_id = " << std::this_thread::get_id() << "DEQUEUE CAS headaim" << endl;
                CAS(tail, curHead, headAim);
            }
        }
        else
        {
            //cout << "thread_id = " << std::this_thread::get_id() << "DEQUEUE if CAS" << endl;
            if (CAS(head, curHead, headAim))
            {
                ans = headAim->value;
                return true;
            }
        }
    }
}

template<typename T>
lock_free_queue<T>::~lock_free_queue()
{
    Node * ptr = nullptr;
    while (start.load() != nullptr)
    {
        ptr = start.load();
        start.exchange(start.load()->aim);
        delete ptr;
    }
}

#endif // LOCKFREE_QUEUE_H
