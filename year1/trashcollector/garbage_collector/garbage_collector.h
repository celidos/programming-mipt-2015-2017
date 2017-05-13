#ifndef SMARTOBJ_H
#define SMARTOBJ_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>

#define DEBUG_OUTPUT

const size_t MAX_HEAP_MEMORY = 1000;

enum ObjType { HEAP_OBJ = false, STACK_OBJ = true };

// ---------------------- SMART OBJECT -----------------------

class SmartObject
{
    friend class GarbageCollector;
public:
    SmartObject();
    virtual std::vector<SmartObject*> pointers() const = 0;

    void* operator new   (size_t size);
    void  operator delete(void* ptr);
    virtual ~SmartObject();
private:
    size_t size_;
};

// ------------------- GARBAGE COLLECTOR ---------------------

class GarbageCollector
{
    friend class SmartObject;
public:
    static GarbageCollector* Instance();
    static bool DeleteInstance();

    static void add   (SmartObject* obj, ObjType place);
    static void remove(SmartObject* obj);

    static void collect();

    static size_t getFreeMem() { return freeMem; }


protected:
    class Element
    {
    public:
        ObjType type;
        bool color;
        SmartObject* ptr;
    };
    std::vector<Element> pointers_;
    static size_t freeMem;
private:
    GarbageCollector();
    ~GarbageCollector();
    static void DFS(SmartObject* v);

    static GarbageCollector* self_;
};


#endif // SMARTOBJ_H

