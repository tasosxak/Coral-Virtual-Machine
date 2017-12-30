#ifndef LITERAL_H
#define LITERAL_H
#include <string>
#include "Data_Type.h"


class Literal : public Data_Type
{
    public:
        Literal();
        Literal(const std::string& value);
        virtual ~Literal();

        std::string getValue();
        void setValue(const std::string& value);

        std::string className();
        std::string toString();

    protected:

    private:
        std::string value;

};

#endif // LITERAL_H
