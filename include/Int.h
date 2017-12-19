#ifndef INT_H
#define INT_H

#include <Integer.h>


class Int : public Integer
{
    public:
        Int();
        virtual ~Int();
        Int(int value);
        Int(Int* symb);
       // Int(const std::string& name,int value);

        int getValue();
        std::string className();
        void setValue(int value);
        std::string print();

    protected:

    private:
        int value;

};

#endif // INT_H
