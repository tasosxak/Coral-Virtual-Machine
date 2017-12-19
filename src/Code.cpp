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

std::string Code::className(){

    return "Code";

}

Code::~Code()
{
    //dtor
}
