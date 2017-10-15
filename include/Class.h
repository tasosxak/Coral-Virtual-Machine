#ifndef CLASS_H
#define CLASS_H

#include <symbol.h>
#include <Object.h>


class Class : public symbol,Object
{
    public:
        Class();
        Class(std::string name);
        virtual ~Class();

    protected:

    private:


};

#endif // CLASS_H
