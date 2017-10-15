#include "hash_table.h"
#include "symbol.h"
#include <string.h>
#include <iostream>

hash_table::hash_table()
{
    this->numSymbols = 0;
    //ctor
}

hash_table::~hash_table()
{
    //dtor
}

void hash_table::add(symbol* symb)
{

    int i;
    symbol *p;

    i=this->mkkey((symb->getName().c_str()));
    std::cout<<"IIIIIIIIII->"<<i<<"\n";
    p=this->arr[i];
    symb->NextSymbol=p;
    symb->PrevSymbol=NULL;
    if(p) p->PrevSymbol=symb;
    this->arr[i]=symb;


    numSymbols++;



}

void hash_table::discard(std::string name)
{

    symbol *symbp;

    symbp = this->lookfor(name);

    if(symbp)
    {

        if(symbp->PrevSymbol != NULL)
            symbp->PrevSymbol->NextSymbol = symbp->NextSymbol;

        if(symbp->NextSymbol != NULL)
            symbp->NextSymbol->PrevSymbol = symbp->PrevSymbol;

        symbp->PrevSymbol = NULL;
        symbp->NextSymbol = NULL;

    }


}


symbol* hash_table::lookfor(std::string name)
{

    int i;
    symbol *p;

    i=this->mkkey(name.c_str());
    p= this->arr[i];

    while(p && (p->getName() != name) )
        p=p->NextSymbol;



    return p;

}

int hash_table::mkkey(const char *s)
{
    const char *p;
    int athr=0;

    for(p=s; *p; p++) athr=athr+(*p);
    return (athr % 500);
}







