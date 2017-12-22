#ifndef FLOAT_H
#define FLOAT_H

#include "Decimal.h"

class Float : public Decimal
{
    public:
        Float();
        virtual ~Float();
        Float(float value);
        Float(Float* symb);
        Float(const std::string& name,float value);
        std::string className();
        float getValue();
        void setValue(float value);
        std::string print();

    protected:

    private:
        float value;
};

#endif // FLOAT_H
