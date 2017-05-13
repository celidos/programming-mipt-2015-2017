#include "garbage_collector.h"
#include <stdio.h>
#include <stdlib.h>

void GarbageCollector::dfs(SmartObject *v) {
    v->marked_ = iterNum_;
    std::vector<SmartObject*> mypointers;
    mypointers = v->pointers();
    for(int i = 0; i < mypointers.size(); i++) {
        if(mypointers[i]->marked_ != iterNum_)
            GarbageCollector::dfs(mypointers[i]);
    }
}

void GarbageCollector::collect() {
    iterNum_++;
    for(auto obj : stackObjects_) {
        GarbageCollector::dfs(obj);
    }
    for(auto obj = otherObjects_.begin(); obj != otherObjects_.end();) {
        if((*obj)->marked_ < iterNum_) {
            auto obj1 = *obj;
            obj++;
            delete obj1;
        }
        else {
            obj++;
        }
    }
}

GarbageCollector::~GarbageCollector() {
    for(auto obj = otherObjects_.begin(); obj != otherObjects_.end();) {
        auto obj1 = *obj;
        obj++;
        delete obj1;
    }
}

SmartObject::~SmartObject() {
    GarbageCollector::Instance().maxMem_ += size_;
    if(onStack_) {
        GarbageCollector::Instance().stackObjects_.erase(this);
    }
    else {
        GarbageCollector::Instance().otherObjects_.erase(this);
    }
}

void SmartObject::operator delete(void* ptr) {
    ((SmartObject*)ptr)->~SmartObject();
    free(ptr);
}

void* SmartObject::operator new(std::size_t size) {
    if(GarbageCollector::Instance().maxMem_ < size) {
        GarbageCollector::Instance().collect();
        if(GarbageCollector::Instance().maxMem_ < size) {
            throw(std::bad_alloc());
        }
    }
    SmartObject* newobj;
    newobj = static_cast<SmartObject*>(malloc(size));
    GarbageCollector::Instance().maxMem_ -= size;
    GarbageCollector::Instance().otherObjects_.insert(newobj);
    newobj->size_ = size;
    return newobj;
}

SmartObject::SmartObject() {
    if(GarbageCollector::Instance().otherObjects_.find(this) == GarbageCollector::Instance().otherObjects_.end()) {
        onStack_ = 1;
        size_ = 0;
        marked_ = 0;
        GarbageCollector::Instance().stackObjects_.insert(this);
    }
    else {
        marked_ = 0;
        onStack_ = 0;
    }
}

