#ifndef ENUMPR_H
#define ENUMPR_H

#include "stack_funks.h" 

enum Registers_name { 
    ROX = 0,
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4
};

struct Register {
    Registers_name reg; //имя 
    type_t reg_val; //значение (то, что в нем хранится)
};

struct Processor {
    Stack_t stk;   
    int code[50] = {};
    int counter = 0; //текщая команда
    Register regs[4] = {}; //массив со структурами              
};

enum Processor_err {
    NO_ERROR                = 0,
    OPENFILE_ERROR          = 1,
    STACK_ERROR             = 2

};




#endif //ENUMPR_H