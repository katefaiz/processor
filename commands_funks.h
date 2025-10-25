#ifndef CF_H
#define CF_H

#include "stack_funks.h" 
#include "processor_struct_enum.h"


Processor_err processor_HLT(Processor *processor);
Processor_err processor_PUSH(Processor *processor); 
Processor_err processor_ADD(Processor *processor);
Processor_err processor_SUB(Processor *processor);
Processor_err processor_MUL(Processor *processor);
Processor_err processor_DIV(Processor *processor);
Processor_err processor_SQRT(Processor *processor);
Processor_err processor_OUT(Processor *processor);
Processor_err processor_PUSHR(Processor *processor);
Processor_err processor_POPR(Processor *processor);
Processor_err processor_JMP(Processor *processor);
Processor_err processor_CALL(Processor *processor); 
Processor_err processor_RET(Processor *processor); 
Processor_err processor_destroy(Processor *processor);
Processor_err processor_PUSHM(Processor *processor);
Processor_err processor_POPM(Processor *processor);
Processor_err processor_DRAW(Processor *processor);

#endif //CF_H