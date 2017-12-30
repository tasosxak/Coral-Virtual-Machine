#include "Literal.h"
#include <string>

Literal::Literal()
{
    //ctor
}

Literal::~Literal()
{
    //dtor
}


Literal::Literal(const std::string& value):Data_Type(){

    this->value =value;
}

std::string Literal::getValue(){

    return this->value;
}

std::string Literal::className(){

    return "Literal";
}

void Literal::setValue(const std::string& value){

   this->value = value;
}

 std::string Literal::toString(){

    return value;
 }
