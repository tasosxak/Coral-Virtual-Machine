#include "Double.h"

Double::Double()
{
    //ctor
}

Double::~Double()
{
    //dtor
}



Double::Double(double value)
{
    this->value = value;
}

double Double::getValue(){

    return this->value;
}

void Double::setValue(double value){

    this->value = value;
}




Double::Double(const std::string& name, double value):symbol(name){

    this->value = value;

}
