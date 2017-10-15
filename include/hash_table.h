
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "symbol.h"

class symbol;

class hash_table
{
    public:
        hash_table();
        virtual ~hash_table();
        void add(symbol* symb);
        void discard(std::string name);
        symbol* lookfor(std::string name);

    protected:

    private:
        int mkkey(const char *s);
        int numSymbols;
        symbol *arr[500];

};

#endif // HASH_TABLE_H
