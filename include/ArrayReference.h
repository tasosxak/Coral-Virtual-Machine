#ifndef ARRAYREFERENCE_H
#define ARRAYREFERENCE_H

#include "Reference.h"
#include "Array.h"

class ArrayReference: public Reference
{
    public:
        ArrayReference();
        ArrayReference(std::string name,Class* classdef, Array* obj,int dimension);
        virtual ~ArrayReference();



    protected:

    private:
        int dimension;

};

#endif // ARRAYREFERENCE_H
