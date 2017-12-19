#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "Object.h"
#include "Heap.h"
class Memory_Manager
{
    public:
        Memory_Manager(long memory,int threshold);
        virtual ~Memory_Manager();
        void add_object(Object* obj);
        void remove_object_reference(Object* obj);

    protected:

    private:
        int threshold;
        Allocated_Object* FREE_SPACE[];
        Heap* heap_memory;

};

#endif // MEMORY_MANAGER_H
