#include "ArrayReference.h"

ArrayReference::ArrayReference()
{
    //ctor
}

ArrayReference::ArrayReference(std::string name,Class* classdef, Array* obj,int dimension):Reference(name,classdef,obj)
{

this->dimension = dimension;

}

ArrayReference::~ArrayReference()
{
    //dtor
}
