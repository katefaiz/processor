#ifndef AS_H
#define AS_H

#include "stack_funks.h" 
#include "processor_struct_enum.h"

Registers_name comparing_registers(const char *reg_name);
Processor_err asm_commands_data(int *program);
Processor_err disasm_commands_data(int program[]);
Processor_err bite_code_file(int program[], int size);
Commands comparing_commands(const char *command);

//void stack_calculate (Stack_t *stk, char *command, int value);

#endif //AS_H