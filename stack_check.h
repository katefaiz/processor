
#ifndef CHECK_H
#define CHECK_H

#include "struct_enum.h"

Stack_err_t stack_verify(Stack_t * stk);
Stack_err_t stack_dump(Stack_t *stk, Stack_err_t err);
void stack_output_err(Stack_err_t err);
void stack_fill_poizon(Stack_t *stk);
void clear_enter (void);


#endif //CHECK_H