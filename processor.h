#ifndef PR_H
#define PR_H



#include "commands_funks.h" 
#include "assembler.h"

Processor_err bite_code_read(Processor *processor);
Processor_err run_bytecode(Processor *processor);
Processor_err processor_init(Processor *processor, type_t capacity);
Processor_err processor_dump(Processor *processor);

#endif //PR_H
