#include "Allocated_Object.h"

Allocated_Object::Allocated_Object(Object* obj,int references){

this->references = references;
this->obj = obj;
this->age = 0;

}

void Allocated_Object::add_refer(){

this->references++;

}

void Allocated_Object::birthday(){

this->age++;
}


void Allocated_Object::remove_refer(){

this->age--;
}

int Allocated_Object::get_age(){

return this->age;
}

int Allocated_Object::get_num_refer(){

return this->references;
}


Allocated_Object::~Allocated_Object()
{
    //dtor
}
