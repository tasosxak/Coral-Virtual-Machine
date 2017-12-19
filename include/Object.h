#ifndef OBJECT_H
#define OBJECT_H

#include <symbol.h>
#include <Class.h>

class Class;

class Object : public symbol
{
    public:
        Object();
        virtual ~Object();

        Class* getClass();
        void setClass(Class* classRef);

    protected:

    private:
        Class* classRef;
};

#endif // OBJECT_H
