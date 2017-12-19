#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <symbol.h>


class Data_Type : public symbol
{
    public:
        Data_Type();
        Data_Type(std::string name);
        virtual ~Data_Type();
        //std::string className();


    protected:

    private:
};

#endif // DATA_TYPE_H
