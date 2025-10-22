#ifndef AS_H
#define AS_H

#include "stack_funks.h" 
#include "processor_struct_enum.h"

Processor_err assembler_init(Assembler *assembler);
Processor_err assembler_compile(Assembler *assembler);
Processor_err assembler_resolve_labels(Assembler *assembler);
Processor_err assembler_save_to_file(Assembler *assembler);
Processor_err assembler_destroy(Assembler *assembler);
Registers_name comparing_registers(const char *reg_name);
Commands comparing_commands(const char *command);
Processor_err disasm_commands_data(int program[]);
//void stack_calculate (Stack_t *stk, char *command, int value);

#endif //AS_H