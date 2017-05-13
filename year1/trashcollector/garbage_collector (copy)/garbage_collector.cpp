#include <garbage_collector.h>

// ----------------- GARBAGE COLLECTOR --------------------

GarbageCollector* GarbageCollector::self_ = nullptr;

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
        printf("_________________________________\n");
        delete self_;
        self_ = nullptr;
        return true;
    }

    return false;
}

void GarbageCollector::add(SmartObject* obj, ObjType place)
{
    for (size_t i = 0; i < self_->pointers_.size(); ++i)
    {
        if (self_->pointers_[i].ptr == obj)
            return;
    }

    self_->pointers_.push_back(Element{ place, false, obj });
    printf("New obj pushed (%s)\n", (place ? "stack" : "heap"));
}

void GarbageCollector::remove(SmartObject* obj)
{
    for (size_t i = 0; i < self_->pointers_.size(); ++i)
    {
        if (self_->pointers_[i].ptr == obj)
        {
            self_->pointers_.erase(self_->pointers_.begin() + i);
            printf("Removed obj\n");
            break;
        }
    }
}

void GarbageCollector::collect()
{
    printf("??????????????????????????????????????\n");
    for (size_t i = 0; i < self_->pointers_.size(); ++i)
    {
        if (self_->pointers_[i].type == STACK_OBJ)
        {
            DFS(self_->pointers_[i].ptr);
        }
    }

    for (size_t i = self_->pointers_.size() - 1; i >= 0; --i)
    {
        for (size_t m = 0; m < self_->pointers_.size(); ++m)
        {
            printf("%d ", self_->pointers_[m].ptr);
        }
        printf("\n");

        if (self_->pointers_[i].color == false)
        {
            delete self_->pointers_[i].ptr;
        }
        else
        {
            self_->pointers_[i].color = false;
        }
    }
    printf("Collecting finished\n");
}

GarbageCollector::~GarbageCollector()
{
    printf("::::::::: ~GarbageCollector, cleaning ::::::::::: \n");
    for (auto obj = pointers_.begin(); obj != pointers_.end();)
    {
        auto obj1 = obj;
        obj++;

        if (obj1->type != STACK_OBJ)
            delete &(*obj1);

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
    printf("SmartObject Constructor\n");
    auto it = GarbageCollector::Instance()->pointers_.begin();

    for (it  = GarbageCollector::Instance()->pointers_.begin();
         it != GarbageCollector::Instance()->pointers_.end(); ++it)
        if ((*it).ptr == this)
            break;

    if (it == GarbageCollector::Instance()->pointers_.end())
    {
        GarbageCollector::Instance()->add(this, STACK_OBJ);
    }
    else
    {
        (*it).type = HEAP_OBJ;
    }
}

SmartObject::~SmartObject()
{
    printf("SmartObject Destructor\n");
    GarbageCollector::Instance()->maxMem += size_;
    GarbageCollector::Instance()->remove(this);
}

void SmartObject::operator delete(void* ptr)
{
    printf("SmartObject delete\n");
    ((SmartObject*)ptr)->~SmartObject();
    free(ptr);
}

void* SmartObject::operator new(std::size_t size)
{
    printf("SmartObject new\n");
    if (GarbageCollector::Instance()->maxMem < size)
    {
        GarbageCollector::Instance()->collect();

        if (GarbageCollector::Instance()->maxMem < size)
            throw(std::bad_alloc());
    }

    SmartObject* newobj;
    newobj = static_cast<SmartObject *> (malloc(size));
    GarbageCollector::Instance()->maxMem -= size;
    GarbageCollector::Instance()->add(newobj, HEAP_OBJ);
    newobj->size_ = size;
    return newobj;
}
