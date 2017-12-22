#include "Float.h"

Float::Float()
{
    //ctor
}

Float::~Float()
{
    //dtor
}


Float::Float(Float* symb){

  //this->name = symb->getName();
  this->linecode = symb->getLine();
  setValue(symb->getValue());

}

Float::Float(float value)
{
    this->value = value;
}

float Float::getValue(){

    return this->value;
}

void Float::setValue(float value){

    this->value = value;
}


std::string Float::className(){

    return "Float";

}



Float::Float(const std::string& name, float value):Decimal(name){

    this->value = value;

}

std::string Float::print(){

 return std::to_string(value);

}
