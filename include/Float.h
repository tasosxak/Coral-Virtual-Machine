#ifndef FLOAT_H
#define FLOAT_H

#include "Decimal.h"

class Float : public Decimal
{
    public:
        Float();
        Float(float value);
        Float(Float* symb);

        virtual ~Float();

        //Float(const std::string& name,float value);

        float getValue();
        void setValue(float value);

        std::string toString();
        std::string className();

    protected:

    private:
        float value;
};

#endif // FLOAT_H
