#ifndef INTEGER_H
#define INTEGER_H
#include "symbol.h"

class Integer : public symbol
{
    public:
        Integer();
        virtual ~Integer();
        Integer(int value);
        Integer(const std::string& name,int value);
        int getValue();
        void setValue(int value);
    protected:

    private:
        int value;
};

#endif // INTEGER_H
