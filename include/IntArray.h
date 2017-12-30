#ifndef INTARRAY_H
#define INTARRAY_H

#include "Array.h"

class IntArray : public Array
{
    public:
        IntArray(int length,int dimension);
        virtual ~IntArray();

        Object* get_element(int index);
        void set_element(int index, Object* obj);
        void del_element(int index);


    protected:

    private:
};

#endif // INTARRAY_H
