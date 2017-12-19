#include "Double.h"

Double::Double()
{
    //ctor
}

Double::~Double()
{
    //dtor
}

Double::Double(Double* symb){

  //this->name = symb->getName();
  this->linecode = symb->getLine();
  setValue(symb->getValue());

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


std::string Double::className(){

    return "Double";

}



Double::Double(const std::string& name, double value):Decimal(name){

    this->value = value;

}

std::string Double::print(){

 return std::to_string(value);

}
