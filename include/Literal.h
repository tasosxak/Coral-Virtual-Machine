#ifndef LITERAL_H
#define LITERAL_H
#include <string>
#include "symbol.h"


class Literal : public symbol
{
    public:
        Literal();
        Literal(const std::string& value);
        Literal(const std::string& name, const std::string& value);
        virtual ~Literal();
        std::string getValue();
        void setValue(const std::string& value);
    protected:

    private:
        std::string value;

};

#endif // LITERAL_H
