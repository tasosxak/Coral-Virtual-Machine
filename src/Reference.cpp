#include "Reference.h"

Reference::Reference()
{
    //ctor
}

Reference::Reference(std::string name, symbol* symb):symbol(name){

  this->pointer = symb;

  if(this->pointer != NULL)
        this->pointer->mht = new hash_table();


}
std::string Reference::getName(){

   return (this->pointer)->getName();

}

void Reference::setPointer(symb* symb){

  this->pointer =symb;

}

symbol* Reference::getPointer(){

  return this->pointer;
}
Reference::~Reference()
{
    //dtor
}
