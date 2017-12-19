#ifndef ACCESSIBLE_H
#define ACCESSIBLE_H

#include <symbol.h>


class Accessible : public symbol
{
    public:
        Accessible();
        Accessible(std::string name,std::string ACC_FLAG);
        Accessible(std::string name,int ACC_FLAG);
        virtual ~Accessible();
        int get_acc();
        void set_acc(std::string name);
        void set_acc(int code);

    protected:

    private:
        int ACC_FLAG;
};

#endif // ACCESSIBLE_H
