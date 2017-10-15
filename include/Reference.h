#ifndef REFERENCE_H
#define REFERENCE_H
#include <string>
#include "symbol.h"

class Reference: public symbol
{
    public:
        Reference();
        Reference(std::string name,symbol* sym);
        std::string getName();
        symbol* getPointer();
        void Reference::setPointer(symb* symb);
        virtual ~Reference();


    protected:

    private:
        symbol *pointer;
};

#endif // REFERENCE_H
