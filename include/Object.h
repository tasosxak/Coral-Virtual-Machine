#ifndef OBJECT_H
#define OBJECT_H

#include <symbol.h>
//#include <Class.h>

//class Class;

class Object : public symbol
{
    public:
        Object();
        Object(std::string name);
        virtual ~Object();

        virtual Object*  getClass();
        //virtual bool equals(Object* obj);
        virtual std::string toString();
        void setClass(Object* classRef);

    protected:
        std::string object_id;


    private:
        Object* classRef;
};

#endif // OBJECT_H
