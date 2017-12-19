#include "Reference.h"
#include "Class.h"

Reference::Reference()
{
    //ctor
}

Reference::Reference(Reference* symb)
{
    this->name = symb->getName();
    this->classRef = symb->getStaticDef();
    this->pointer = symb->getPointer();
}


Reference::Reference(std::string name,Class* classdef, Object* obj):Numeric(name){

  this->pointer = obj;
  this->classRef = classdef;

}

std::string Reference::getName(){

   return symbol::getName();

}

Class* Reference::getDynamicDef(){

   return (this->pointer)->getClass();

}

std::string Reference::className(){

    return "Reference";
}

void Reference::setPointer(Object* symb){

  this->pointer =symb;

}

void Reference::setStaticDef(Class* cls){

  this->classRef = cls;

}

Object* Reference::getPointer(){

  return this->pointer;
}

Class* Reference::getStaticDef(){

  return this->classRef;
}

Reference::~Reference()
{
    //dtor
}
