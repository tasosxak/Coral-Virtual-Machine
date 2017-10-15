#ifndef VS_STACK_H
#define VS_STACK_H
#include "symbol.h"

class vs_stack
{

    public:
        vs_stack();
        virtual ~vs_stack();
        symbol* pop();
        symbol* see(int index);
        void push(symbol* symb);
        int esp();
        void setesp(int newesp);
    protected:

    private:
        int counter = 0;
        int LENGTH_ST;
        symbol* arr[65000];
};

#endif // VS_STACK_H
