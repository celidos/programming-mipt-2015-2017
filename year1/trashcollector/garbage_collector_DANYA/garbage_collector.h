#ifndef SMARTOBJ_H
#define SMARTOBJ_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <set>

class SmartObject
{
friend class GarbageCollector;
public:
    SmartObject();

    virtual std::vector<SmartObject *> pointers() const = 0; // Should be implemented by user

    void* operator new(std::size_t size);

    void operator delete(void* ptr);

    virtual ~SmartObject();

private:
    bool onStack_;
    int marked_;
    int size_;
};

class GarbageCollector
{
friend class SmartObject;
public:
    static GarbageCollector& Instance() {
        static GarbageCollector theSingleInstance;
        return theSingleInstance;
    }

    virtual void collect();

    virtual ~GarbageCollector();

private:
    GarbageCollector(){}
    GarbageCollector(const GarbageCollector& root);
    GarbageCollector& operator=(const GarbageCollector&);
    int maxMem_ = 10000;
    int iterNum_ = 0;
    std::set<SmartObject *> stackObjects_;
    std::set<SmartObject *> otherObjects_;
    virtual void dfs(SmartObject *v);
};


#endif // SMARTOBJ_H

