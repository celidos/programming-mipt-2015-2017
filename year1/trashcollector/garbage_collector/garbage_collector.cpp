#include <garbage_collector.h>

// ----------------- GARBAGE COLLECTOR --------------------

GarbageCollector* GarbageCollector::self_ = nullptr;
size_t GarbageCollector::freeMem = MAX_HEAP_MEMORY;

GarbageCollector* GarbageCollector::Instance()
{
    if (!self_)
        self_ = new GarbageCollector();

    return self_;
}

bool GarbageCollector::DeleteInstance()
{
    if (self_)
    {
        delete self_;
        self_ = nullptr;
        return true;
    }

    return false;
}

void GarbageCollector::add(SmartObject* obj, ObjType place)
{
    for (size_t i = 0; i < self_->pointers_.size(); ++i)
        if (self_->pointers_[i].ptr == obj)
            return;

    self_->pointers_.push_back(Element{ place, false, obj });
#ifdef DEBUG_OUTPUT
    printf("New obj pushed (%s)\n", (place ? "stack" : "heap"));
#endif
}

void GarbageCollector::remove(SmartObject* obj)
{

    for (size_t i = 0; i < self_->pointers_.size(); ++i)
    {
        if (self_->pointers_[i].ptr == obj)
        {
            self_->pointers_.erase(self_->pointers_.begin() + i);
#ifdef DEBUG_OUTPUT
            printf("GarbageCollector remove (with conditions)\n");
#endif
            break;
        }
    }
}

void GarbageCollector::collect()
{
#ifdef DEBUG_OUTPUT
    printf("                    GarbageCollector::collect\n");
#endif
    for (size_t i = 0; i < self_->pointers_.size(); ++i)
    {
        if (self_->pointers_[i].type == STACK_OBJ)
        {
            DFS(self_->pointers_[i].ptr);
        }
    }

    for (int i = self_->pointers_.size() - 1; i >= 0; --i)
    {

        if (self_->pointers_[i].color == false)
            delete self_->pointers_[i].ptr;
        else
            self_->pointers_[i].color = false;
    }
#ifdef DEBUG_OUTPUT
    printf("                    Collecting finished\n");
#endif
}

GarbageCollector::GarbageCollector()
{
#ifdef DEBUG_OUTPUT
    printf("GarbageCollector::GarbageCollector\n");
#endif
}

GarbageCollector::~GarbageCollector()
{
#ifdef DEBUG_OUTPUT
    printf("GarbageCollector::~GarbageCollector\n");
#endif
    for (int i = self_->pointers_.size() - 1; i >= 0; --i)
    {
        if (self_->pointers_[i].type != STACK_OBJ)
            delete self_->pointers_[i].ptr;
    }
}

void GarbageCollector::DFS(SmartObject* v)
{
    std::vector<SmartObject *> sons = v->pointers();

    for (size_t i = 0; i < self_->pointers_.size(); ++i)
    {
        if (self_->pointers_[i].ptr == v)
        {
            if (self_->pointers_[i].color)
                return;

            self_->pointers_[i].color = true;
            break;
        }
    }

    for (size_t i = 0; i < sons.size(); ++i)
        DFS(sons[i]);
}

// -------------------- SMART OBJECT --------------------

SmartObject::SmartObject()
{
#ifdef DEBUG_OUTPUT
    printf("SmartObject Constructor, size = %d\n", this->size_);
#endif
    auto it = GarbageCollector::Instance()->pointers_.begin();

    for (it  = GarbageCollector::Instance()->pointers_.begin();
         it != GarbageCollector::Instance()->pointers_.end(); ++it)
        if ((*it).ptr == this)
            break;

    if (it == GarbageCollector::Instance()->pointers_.end())
    {
        size_ = 0;
        GarbageCollector::Instance()->add(this, STACK_OBJ);
    }
    else
        (*it).type = HEAP_OBJ;
}

SmartObject::~SmartObject()
{
#ifdef DEBUG_OUTPUT
    printf("SmartObject::~SmartObject\n");
#endif
    GarbageCollector::Instance()->freeMem += size_;
    GarbageCollector::Instance()->remove(this);
}

void SmartObject::operator delete(void * ptr)
{
#ifdef DEBUG_OUTPUT
    printf("SmartObject::operator delete, size = %d\n", ((SmartObject *)ptr)->size_);
#endif
    free(ptr);
}

void* SmartObject::operator new(size_t size)
{
#ifdef DEBUG_OUTPUT
    printf("SmartObject::operator new\n");
    printf(" FreeMemory : %lu\n", GarbageCollector::Instance()->freeMem);
#endif
    if (GarbageCollector::Instance()->freeMem < size)
    {
        GarbageCollector::Instance()->collect();

        if (GarbageCollector::Instance()->freeMem < size)
            throw(std::bad_alloc());
    }

    SmartObject* newobj;
    newobj = static_cast<SmartObject *> (malloc(size));
    newobj->size_ = size;
    GarbageCollector::Instance()->freeMem -= size;
    GarbageCollector::Instance()->add(newobj, HEAP_OBJ);

    return newobj;
}
