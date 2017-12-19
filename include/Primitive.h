#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <Data_Type.h>


class Primitive : public Data_Type
{
    public:
        Primitive();
        Primitive(std::string name);
        virtual ~Primitive();

    protected:

    private:
};

#endif // PRIMITIVE_H
