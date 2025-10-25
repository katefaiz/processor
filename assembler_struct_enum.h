#ifndef ENUMASS_H
#define ENUMASS_H

struct Assembler {
    int program[200] = {};      // байт-код программы
    int labels[50] = {};      // массив меток
    int labels_count = 0;       // количество определенных меток
    int program_size = 0;       // размер байт-кода
    char source_file[50] = "commands_data_curcle.txt"; //commands_data_curcle.txt //commands_data_kvadratka.txt
    char output_file[50] = "bite_code.txt";     // выходной файл
};
enum Registers_name { 
    ROX = 0, //для ошибок
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4
};

// https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent

struct Register {
    Registers_name reg; //имя 
    type_t reg_val; //значение (то, что в нем хранится)
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
    DRAW    = 13, //вывод видеопамяти
    ERROR   = 666 //для ошибки

};
enum Assembler_err {
    ASS_NO_ERROR                = 0,
    ASS_OPENFILE_ERROR          = 1,
    ASS_STACK_ERROR             = 2,
    ASS_LABEL_ERROR             = 3

};

#endif //ENUMASS