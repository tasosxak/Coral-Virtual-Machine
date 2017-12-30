#ifndef METHOD_H
#define METHOD_H

#include <Accessible.h>
#include <hashtable.h>
#include <Object.h>

class Method : public Accessible, public Object
{
    public:
        Method(std::string name,std::string ACC_FLAG, std::string RTYPE);
        virtual ~Method();
        void set_rtype(std::string rtype);
        std::string get_rtype();


    protected:

    private:
        std::string rtype;

};

#endif // METHOD_H
