#include "Int.h"

Int::Int()
{
    //ctor
}

Int::~Int()
{
    //dtor
}

Int::Int(Int* symb){

  //this->name = symb->getName();
  this->linecode = symb->getLine();
  setValue(symb->getValue());

}

Int::Int(int value)
{
    this->value = value;
}

int Int::getValue(){

    return this->value;
}

void Int::setValue(int value){

    this->value = value;
}

std::string Int::className(){

    return "Int";
}

std::string Int::toString(){

 return std::to_string(value);

}

/*
Int::Int(const std::string& name, int value):Integer(name){

    this->value = value;

}
*/
