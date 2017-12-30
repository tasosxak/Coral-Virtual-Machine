#ifndef C_ENTITY_H
#define C_ENTITY_H

#include "symbol.h"

class c_entity: public symbol
{
    public:
        c_entity();
        c_entity(std::string name);
        virtual ~c_entity();

    protected:

    private:
};

#endif // C_ENTITY_H
