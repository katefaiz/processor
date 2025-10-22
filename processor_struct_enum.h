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
    JMP     = 8, //перепрыг на нужный адрес
    CALL    = 9, // вызов функции
    RET     = 10, // возврат из функции
    POPM    = 11, //удаляю из стека и кладу в оперативную память 
    PUSHM   = 12,
    POPR    = 42, // удаляю из стека и кладу в регистр
    PUSHR   = 33, //кладу в стек из регистра, в регистре остается
    ERROR   = 666 //для ошибки

};

struct Register {
    Registers_name reg; //имя 
    type_t reg_val; //значение (то, что в нем хранится)
};

struct Assembler {
    int program[100] = {};      // байт-код программы
    int labels[20] = {};      // массив меток
    int labels_count = 0;       // количество определенных меток
    int program_size = 0;       // размер байт-кода
    char source_file[50] = "commands_data.txt"; // исходный файл
    char output_file[50] = "bite_code.txt";     // выходной файл
};

struct Processor {
    Stack_t stk;  
    Stack_t call_stack; 
    int code[100] = {};
    int counter = 0; //текщая команда
    Register regs[5] = {};   
    int RAM[100] = {};
};

enum Processor_err {
    NO_ERROR                = 0,
    OPENFILE_ERROR          = 1,
    STACK_ERROR             = 2,
    LABEL_ERROR             = 3

};

#endif //ENUMPR_H