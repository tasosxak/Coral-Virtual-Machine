#include "Object.h"

Object::Object()
{
    //ctor
}

Object::Object(std::string name):symbol(name)
{
    //ctor
}


Object::~Object()
{
    //dtor
}

Object* Object::getClass(){

  return this->classRef;

 }

void Object::setClass(Object* classRef){

 this->classRef = classRef;
}

std::string Object::toString(){

return object_id;

}
