#ifndef ALLOCATED_OBJECT_H
#define ALLOCATED_OBJECT_H

#include "Object.h"

class Allocated_Object
{
    public:
        Allocated_Object(Object* obj,int references);
        virtual ~Allocated_Object();
        void add_refer();
        void remove_refer();

        int get_age();
        int get_num_refer();

        void birthday();

    protected:

    private:
        Object* obj;
        int references;
        int age;
};

#endif // ALLOCATED_OBJECT_H
