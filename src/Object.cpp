#include "Object.h"

Object::Object()
{
    //ctor
}


Object::~Object()
{
    //dtor
}

 Class* Object::getClass(){

  return this->classRef;
 }

void Object::setClass(Class* classRef){

 this->classRef = classRef;
}
