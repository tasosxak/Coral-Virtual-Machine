#ifndef SYMBOL_H
#define SYMBOL_H
#include <string>
#include "hash_table.h"

class hash_table;

class symbol
{
    public:
        symbol();
        symbol(std::string name);
        virtual ~symbol();
        std::string getName();
        void setLine(int line);
        int getLine();
        symbol *NextSymbol;
        symbol *PrevSymbol;
        hash_table *mht;

    protected:

    private:
        std::string name;
        int linecode;

};

#endif // SYMBOL_H
