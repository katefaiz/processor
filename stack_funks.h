#ifndef FUNKS_H
#define FUNKS_H

#include "struct_enum.h"
#include "stack_check.h"
Stack_err_t stack_destroy(Stack_t *stk);
Stack_err_t stack_init(Stack_t *stk, type_t capasity);
Stack_err_t stack_push(Stack_t *stk, type_t valie);    
Stack_err_t stack_pop(Stack_t *stk, type_t *err);


#endif // FUNKS_H