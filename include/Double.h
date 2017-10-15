#ifndef DOUBLE_H
#define DOUBLE_H
#include "symbol.h"

class Double : public symbol
{
    public:
        Double();
        virtual ~Double();
        Double(double value);
        Double(const std::string& name,double value);
        double getValue();
        void setValue(double value);
    protected:

    private:
        double value;
};

#endif // DOUBLE_H
