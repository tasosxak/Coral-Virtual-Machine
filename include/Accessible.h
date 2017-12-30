#ifndef ACCESSIBLE_H
#define ACCESSIBLE_H
#include <iostream>

class Accessible
{
    public:
        Accessible();
        Accessible(std::string ACC_FLAG);
        Accessible(int ACC_FLAG);
        virtual ~Accessible();
        int get_acc();
        void set_acc(std::string name);
        void set_acc(int code);

    protected:

    private:
        int ACC_FLAG;
};

#endif // ACCESSIBLE_H
