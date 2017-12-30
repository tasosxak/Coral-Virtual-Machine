#include "Method.h"

Method::Method(std::string name,std::string ACC_FLAG, std::string RTYPE):Object(name),Accessible(ACC_FLAG)
{

    set_rtype(RTYPE);
}

void Method::set_rtype(std::string rtype){

    this->rtype = rtype;
}

std::string Method::get_rtype(){

    return rtype;
}

Method::~Method()
{
    //dtor
}
