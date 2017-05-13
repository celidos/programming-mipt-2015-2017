#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H

#include <thread>
#include <atomic>

#define CAS(x, y, z) x.compare_exchange_strong((y), (z))

using std::atomic;

unsigned const MAX_HAZARD_POINTERS = 1000;

// ======================== HAZARD POINTER ===========================
//                           (from github)

struct HazardPointer
{
    std::atomic<std::thread::id> id;
    std::atomic<void*> pointer;
};
HazardPointer g_hazardPointers[MAX_HAZARD_POINTERS];

class HPOwner
{
public:
    HPOwner(const HPOwner&) = delete;
    HPOwner operator=(const HPOwner&) = delete;

    HPOwner()
            : hp_(nullptr)
    {
        for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
            std::thread::id oldId;
            if (g_hazardPointers[i].id.compare_exchange_strong(
                    oldId, std::this_thread::get_id())) {
                hp_ = &g_hazardPointers[i];
                break;
            }
        }
        if (!hp_) {
            throw std::runtime_error("No hazard pointers available");
        }
    }

    std::atomic<void*>& getPointer()
    {
        return hp_->pointer;
    }

    ~HPOwner()
    {
        hp_->pointer.store(nullptr);
        hp_->id.store(std::thread::id());
    }

private:
    HazardPointer* hp_;
};

std::atomic<void*>& getHazardPointerForCurrentThread()
{
    thread_local static HPOwner hazard;
    return hazard.getPointer();
}

bool outstandingHazardPointersFor(void* p)
{
    for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
        if (g_hazardPointers[i].pointer.load() == p) {
            return true;
        }
    }
    return false;
}

template<typename T>
void doDelete(void* p)
{
    delete static_cast<T*>(p);
}

struct DataToReclaim
{
    void* data;
    std::function<void(void*)> deleter;
    DataToReclaim* next;

    template<typename T>
    DataToReclaim(T* p)
            : data(p)
            , deleter(&doDelete<T>)
            , next(nullptr)
    {}

    ~DataToReclaim()
    {
        deleter(data);
    }
};

std::atomic<DataToReclaim*> nodesToReclaim;

void addToReclaimList(DataToReclaim* node)
{
    node->next = nodesToReclaim.load();
    while (!nodesToReclaim.compare_exchange_weak(node->next, node));
}

template<typename T>
void reclaimLater(T* data)
{
    addToReclaimList(new DataToReclaim(data));
}

void deleteNodesWithNoHazards()
{
    DataToReclaim* current = nodesToReclaim.exchange(nullptr);
    while (current) {
        DataToReclaim* const next = current->next;
        if (!outstandingHazardPointersFor(current->data)) {
            delete current;
        } else {
            addToReclaimList(current);
        }
        current = next;
    }
}

// ===================== LOCK FREE QUEUE =========================

template <typename T>
class lock_free_queue
{
public:
    lock_free_queue();
    ~lock_free_queue();

    void enqueue(T v);
    bool dequeue(T &item);

private:
    struct Node {
        Node()             { aim = nullptr; }
        Node(T newvalue)   { value = newvalue; aim = nullptr; }
        Node(T &&newvalue) { value = newvalue; aim = nullptr; }

        atomic<Node*> aim;
        T value;
    };

    atomic<Node*> head;
    atomic<Node*> tail;

    //void hazardSearching(Node *sourceNext, atomic<Node*> &source);
};

template<typename T>
lock_free_queue<T>::lock_free_queue()
{
    head.store(new Node());
    tail.store(head);
}



template<typename T>
void lock_free_queue<T>::enqueue(T v)
{
    Node *newnode = new Node(v);
    Node *curTail;
    Node *tailNext;

    //cout << "thread_id = " << std::this_thread::get_id() << "ENQUEUE" << endl;

    while (true)
    {
        //cout << "thread_id = " << std::this_thread::get_id() << "~~~~~~~ ANOTHER ITERATION ~~~~~~~~~" << endl;
        curTail = tail.load();

        Node *iter = curTail;
        do
        {
            iter = curTail;
            //cout << "thread_id = " << std::this_thread::get_id() << " :  getHazardPointerForCurrentThread().store" << endl;
            getHazardPointerForCurrentThread().store(iter);
            curTail = tail.load();
        } while (iter != curTail); //while (iter == curTail);

        // Node *iter = curTail;
        // while (true)
        //{
        //    iter = curTail;
        //      cout << "thread_id = " << std::this_thread::get_id() << " :  getHazardPointerForCurrentThread().store" << endl;
        //
        // }

        //cout << "thread_id = " << std::this_thread::get_id() << " : GET HERE!!!!" << endl;

        tailNext = curTail->aim.load();

        if (!tailNext)
        {
            if (tail.load()->CAS(aim, tailNext, newnode))
                break;
        }
        else
        {
            //cout << "thread_id = " << std::this_thread::get_id() << "ENQUEUE CAS TAIL TAILAIM" << endl;
            CAS(tail, curTail, tailNext);
        }
    }

    CAS(tail, curTail, newnode);
    getHazardPointerForCurrentThread().store(nullptr);
}

template<typename T>
bool lock_free_queue<T>::dequeue(T &v)
{
    Node *curHead;
    Node *curTail;
    Node *tailNext;

    while (true)
    {
        //cout << "thread_id = " << std::this_thread::get_id() << "~~~~~~~ ANOTHER ITERATION ~~~~~~~~~" << endl;

        curHead = head.load();

        Node* iter;
        do
        {
            iter = curHead;
            getHazardPointerForCurrentThread().store(iter);
            curHead = head.load();
        }
        while (iter != curHead);

        //cout << "FINITO!" << endl;

        curTail = tail.load();
        tailNext = curHead->aim.load();

        //cout << "thread_id = " << std::this_thread::get_id() << "~~~~~~~ SECON HAZARD ~~~~~~~~~" << endl;

        HPOwner hazardPointerOwner;
        do
        {
            iter = tailNext; //

            hazardPointerOwner.getPointer().store(iter);

            //cout << "thread_id = " << std::this_thread::get_id() << "~~~~~~~ SECON HAZARD ~~~~~~~~~" << endl;

            tailNext = curHead->aim.load();
            //iter = tailNext;
        }
        while (iter != tailNext);

        // Node *iter = curTail;
        // while (true)
         //{
         //    tailNext = curHead->aim.load();
             //cout << "thread_id = " << std::this_thread::get_id() << " :  getHazardPointerForCurrentThread().store" << endl;

        // }

        //if (iter)

        if (curHead == curTail)
        {
            if (!tailNext) {
                //cout << "thread_id = " << std::this_thread::get_id() << "I GOT THIS FUCKING PLACE!!!!!!" << endl;
                getHazardPointerForCurrentThread().store(nullptr);
                return false;
            }
            else
            {
                //cout << "thread_id = " << std::this_thread::get_id() << "DEQUEUE if CAS" << endl;
                CAS(tail, curHead, tailNext);
            }
        }
        else
        {
            // cout << "thread_id = " << std::this_thread::get_id() << "another case!!!!!!!!!!!!!" << endl;
            if (CAS(head, curHead, tailNext)) {
                v = tailNext->value;

                reclaimLater(curHead);
                deleteNodesWithNoHazards();
                getHazardPointerForCurrentThread().store(nullptr);

                //cout << "thread_id = " << std::this_thread::get_id() << "returning true" << endl;
                return true;
            }
        }
    }
}

template<typename T>
lock_free_queue<T>::~lock_free_queue()
{
    //Node * ptr = nullptr;
    //while (start.load() != nullptr)
    //{
    //    ptr = start.load();
    //    start.exchange(start.load()->aim);
    //    delete ptr;
    //}
}

#endif
