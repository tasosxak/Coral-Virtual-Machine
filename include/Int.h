#ifndef INT_H
#define INT_H

#include <Integer.h>


class Int : public Integer
{
    public:
        Int();
        Int(int value);
        Int(Int* symb);

        virtual ~Int();
       // Int(const std::string& name,int value);

        int getValue();
        void setValue(int value);

        std::string toString();
        std::string className();

    protected:

    private:
        int value;

};

#endif // INT_H
