#include "Field.h"
#include "Int.h"
#include "Double.h"
#include "Reference.h"

Field::Field(std::string name, std::string ACC_FLAG , Data_Type* data): Accessible(name,ACC_FLAG){

    set_data(data);
}

Field::Field(Field* field):Accessible(field->getName(), field->get_acc()){



    std::string className = (field->get_data())->className();

    if(className == "Int"){

        Data_Type* x = field->get_data();
        Int* y = (Int*)x;
        set_data(new Int(y));
    }
    else if (className == "Double"){

        set_data(new Double((Double*)field->get_data()));
    }
    else if (className == "Reference"){

        set_data(new Reference((Reference*)field->get_data()));
    }

}

std::string Field::print(){

 return data->print();
}
Data_Type* Field::get_data(){

    return this->data;
}

void Field::set_data(Data_Type* data){

    this->data = data;
}


std::string Field::className(){

return "Field";
}

Field::~Field()
{
    //dtor
}
