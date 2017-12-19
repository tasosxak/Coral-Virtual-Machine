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
        symbol(symbol* symb);
        virtual ~symbol();
        std::string getName();
        void setLine(int line);
        int getLine();
        virtual std::string className();
        virtual std::string print();

        symbol *NextSymbol;
        symbol *PrevSymbol;
        hash_table *mht;

    protected:
        std::string name;
        int linecode;
    private:


};

#endif // SYMBOL_H
