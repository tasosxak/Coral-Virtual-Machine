#ifndef CVM_H
#define CVM_H

#include <iostream>

#include "vs_stack.h"
#include "Class.h"

using namespace std;

class CVM{

public:
    CVM(std::string bytecodefile);
    virtual ~CVM();
    int start();

protected:

private:
    std::string bytecodefile;

    //Basic Registers
    int eoc = -1; // End of code
    int ic = 0; // Instruction Counter
    int ebp = 0; // Stack Frame

    vs_stack<symbol> st; // Variable Stack
    vs_stack<hash_table> hts; // Symbol Table Stack

    hash_table hs; // Global Symbol Table
    hash_table class_defs; // Class Definitions Pool

    hash_table* cur_hs = &hs; //Current symbol table
    Class* cur_class; //Current class

    int Loader(string filename);
    template <class Container>
    void split(const std::string& str, Container& cont);
    int compile();
    int execute();

};

#endif // CVM_H
