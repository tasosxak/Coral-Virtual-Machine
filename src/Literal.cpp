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


Literal::Literal(const std::string& value){

    this->value = value;

 }

Literal::Literal(const std::string& name, const std::string& value):symbol(name){

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

 std::string Literal::print(){

    return value;
 }
