#include "symbol.h"

symbol::symbol()
{
    this->NextSymbol = NULL;
    this->PrevSymbol = NULL;
    this->mht = NULL;
    //ctor
}
symbol::symbol(std::string name){

    this->name = name;

    this->NextSymbol = NULL;
    this->PrevSymbol = NULL;
    this->mht = NULL;

}

std::string symbol::getName(){
    std::string str(this->name);
    return str;

}

int symbol::getLine(){

    return this->linecode;
}

void symbol::setLine(int line){
    this->linecode = line;
}

symbol::~symbol()
{
    //dtor
}
