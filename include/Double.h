#ifndef DOUBLE_H
#define DOUBLE_H
#include "Decimal.h"

class Double : public Decimal
{
    public:
        Double();
        virtual ~Double();
        Double(double value);
        Double(Double* symb);
        Double(const std::string& name,double value);
        std::string className();
        double getValue();
        void setValue(double value);
        std::string print();

    protected:

    private:
        double value;
};

#endif // DOUBLE_H
