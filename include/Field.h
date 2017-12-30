#ifndef FIELD_H
#define FIELD_H

#include <Accessible.h>
#include <Data_Type.h>
#include <Object.h>

class Field : public Accessible, public Object
{
    public:
        Field(std::string name, std::string ACC_FLAG , Data_Type* data);
        Field(Field* field);
        virtual std::string className();
        virtual ~Field();
        Data_Type* get_data();
        void set_data(Data_Type* data);

    protected:

    private:
        Data_Type* data;
};

#endif // FIELD_H
