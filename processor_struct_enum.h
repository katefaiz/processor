#ifndef ENUMPR_H
#define ENUMPR_H

#include "stack_funks.h" 

enum Registers_name { 
    ROX = 0, //для ошибок
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4
};

enum Commands {
    HLT     = 0,
    PUSH    = 1, //добавление эл-та
    ADD     = 2, //сложение
    SUB     = 3, //вычитание
    MUL     = 4, //умножение
    DIV     = 5, //деление
    OUT     = 6, //вывод последнего элемента через pop
    SQRT    = 7, //квадратный корень
    JMP     = 8, 
    POPR    = 42, // удаляю из стека и кладу в регистр
    PUSHR   = 33, //кладу в стек из регистра, в регистре остается
    ERROR   = 666 //для ошибки

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