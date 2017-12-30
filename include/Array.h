#ifndef ARRAY_H
#define ARRAY_H
#include "Data_Type.h"

class Array: public Data_Type
{
    public:
        Array();
        virtual ~Array();

        virtual Object* get_element(int index);
        virtual void set_element(int index, Object* obj);
        virtual void del_element(int index);

        int get_length();
        int get_dimension();



    protected:
        Object** arr;
        int length;
        int dimension;

    private:
};

#endif // ARRAY_H
