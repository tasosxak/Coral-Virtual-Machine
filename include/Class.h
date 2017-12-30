#ifndef CLASS_H
#define CLASS_H

#include <Accessible.h>
#include <Object.h>

class Object;

class Class : public Object , public Accessible
{
    public:
        Class();
        Class(std::string name, std::string ACC_FLAG,Class* supercl);
        Class(Class* obj);
        virtual ~Class();
        std::string className();
        Object* createInstance();
        Class* getSuperClass();
        void setSuperClass(Class* supercl);

    protected:


    private:
        Class* superclass;


};

#endif // CLASS_H
