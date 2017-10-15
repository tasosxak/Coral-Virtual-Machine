#include "Code.h"


Code::Code()
{
    //ctor
}

Code::Code(const std::string& code){

    this->code = code;

}

std::string Code::getCode(){

    return this->code;

}
Code::~Code()
{
    //dtor
}
