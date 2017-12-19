#include "Accessible.h"

Accessible::Accessible()
{
    //ctor
}

Accessible:: Accessible(std::string name , std::string ACC_FLAG) : symbol(name)
{
    set_acc(ACC_FLAG);
}

Accessible:: Accessible(std::string name , int ACC_FLAG) : symbol(name)
{
    set_acc(ACC_FLAG);
}

Accessible::~Accessible()
{
    //dtor
}

int Accessible::get_acc(){

    return this->ACC_FLAG;

}

void Accessible::set_acc(int code) {

    ACC_FLAG = code;

}

void Accessible::set_acc(std::string name) {


    if(name == "public")
        ACC_FLAG = 0;
    else if(name == "protected")
        ACC_FLAG = 1;
    else if(name == "private")
        ACC_FLAG = 2;

}
