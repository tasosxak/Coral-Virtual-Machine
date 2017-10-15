#include "vs_stack.h"

vs_stack::vs_stack()
{
    this->counter = -1;
    this->LENGTH_ST = 65000;
}

void vs_stack::push(symbol* symb){

    if(counter < LENGTH_ST -1 ){

        this->arr[++counter] = symb;

    }


}

symbol* vs_stack::see(int index){

    if(index >= 0 && index < LENGTH_ST)
        return this->arr[index];

}

symbol* vs_stack::pop(){

    if(counter >= 0){

        return this->arr[counter--];
    }
}

int vs_stack::esp() {

    return this->counter;

}

void vs_stack::setesp(int newesp){

    this->counter = newesp;
}

vs_stack::~vs_stack()
{
    //dtor
}
