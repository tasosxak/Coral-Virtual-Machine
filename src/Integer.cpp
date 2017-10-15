#include "Integer.h"

Integer::Integer()
{
    //ctor
}

Integer::Integer(int value)
{
    this->value = value;
}

int Integer::getValue(){

    return this->value;
}

void Integer::setValue(int value){

    this->value = value;
}


Integer::~Integer()
{
    //dtor
}

Integer::Integer(const std::string& name, int value):symbol(name){

    this->value = value;

}
