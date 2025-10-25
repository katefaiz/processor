#ifndef ENUMPR_H
#define ENUMPR_H

#include "stack_funks.h" 
#include "assembler_struct_enum.h"

// enum Registers_name { 
//     ROX = 0, //для ошибок
//     RAX = 1,
//     RBX = 2,
//     RCX = 3,
//     RDX = 4
// };

// struct Register {
//     Registers_name reg; //имя 
//     type_t reg_val; //значение (то, что в нем хранится)
// };

struct Processor {
    Stack_t stk;  
    Stack_t call_stack; 
    int code[100] = {};
    int counter = 0; //текщая команда
    Register regs[5] = {};   
    type_t RAM[100] = {};
    type_t video_RAM[100];
};

enum Processor_err {
    NO_ERROR                = 0,
    OPENFILE_ERROR          = 1,
    STACK_ERROR             = 2,
    LABEL_ERROR             = 3

};

#endif //ENUMPR