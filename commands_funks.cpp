#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "processor_struct_enum.h"
#include "stack_check.h"
#include "stack_funks.h"
#include "commands_funks.h"
 

Processor_err processor_HLT(Processor *processor) {
    printf("Конец\n");
    //processor_destroy(processor); 
    return NO_ERROR;
}
Processor_err processor_PUSH(Processor *processor) {
    //processor->counter++; // переходим к значению
    int value = processor->code[processor->counter + 1];
    
    Stack_err_t err = stack_push(&processor->stk, value);
    if (err != STACK_NO_ERROR) {
        
        stack_dump(&processor->stk, err);
        stack_destroy(&processor->stk);
        return STACK_ERROR;
    }
    //printf("PUSH: добавлено значение %d\n", value); 
    return NO_ERROR;

}

Processor_err processor_ADD(Processor *processor) {
    type_t val1 = 0, val2 = 0;
    stack_pop(&processor->stk, &val1);
    stack_pop(&processor->stk, &val2);
    stack_push(&processor->stk, val1 + val2);
    return NO_ERROR;
    
}

Processor_err processor_SUB(Processor *processor) {
    type_t val1 = 0, val2 = 0;
    stack_pop(&processor->stk, &val1);
    stack_pop(&processor->stk, &val2);
    stack_push(&processor->stk, val2 - val1);
    return NO_ERROR;
    
}

Processor_err processor_MUL(Processor *processor) {
    type_t val1 = 0, val2 = 0;
    stack_pop(&processor->stk, &val1);
    stack_pop(&processor->stk, &val2);
    stack_push(&processor->stk, val1 * val2);
    return NO_ERROR;

}

Processor_err processor_DIV(Processor *processor) {
    type_t val1 = 0, val2 = 0;
    stack_pop(&processor->stk, &val1);
    stack_pop(&processor->stk, &val2);
    if (val2 == 0) {
        printf("Ошибка: деление на ноль\n");
        stack_destroy(&processor->stk);
        return STACK_ERROR;
    }
    stack_push(&processor->stk, val2 / val1);
    return NO_ERROR;
}

Processor_err processor_SQRT(Processor *processor) {
    type_t val = 0;
    stack_pop(&processor->stk, &val);
    if (val < 0) {
        printf("Ошибка: корень из отрицательного числа\n");
        stack_destroy(&processor->stk);
        return STACK_ERROR;
    }
    stack_push(&processor->stk, (type_t)sqrt(val));
    return NO_ERROR;

}

Processor_err processor_OUT(Processor *processor) {
    type_t val = 0;
    stack_pop(&processor->stk, &val);
    printf("OUT: %d\n", val);
    return NO_ERROR;

}





Processor_err processor_POPR(Processor *processor) { // удаляю из стека и кладу в регистр
    type_t val = 0;
    int num_reg = processor->code[processor->counter + 1];
    stack_pop(&processor->stk, &val);
    processor->regs[num_reg].reg_val = val;
    //const char* reg_names[] = {"ROX", "RAX", "RBX", "RCX", "RDX"};
    //printf("POPR: значение %d записано в регистр %s\n", val, reg_names[num_reg]);
    return NO_ERROR;
    
}

Processor_err processor_PUSHR(Processor *processor) { //кладу в стек из регистра, в регистре остается
    int num_reg = processor->code[processor->counter + 1];
    type_t val = processor->regs[num_reg].reg_val;
    stack_push(&processor->stk, val);
    //const char* reg_names[] = {"ROX", "RAX", "RBX", "RCX", "RDX"};
    //printf("PUSHR: значение %d записано в регистр %s\n", val, reg_names[num_reg]);
    return NO_ERROR;
    
}

Processor_err processor_JMP(Processor *processor) {
    
    int jump_address = processor->code[processor->counter + 1]; //адрес, куда нужно прыгнуть
    
    processor->counter = jump_address ;
    
    printf("JMP: переход на адрес %d\n", jump_address);
    
    printf("\n=== Состояние стека после JMP ===\n");
    Stack_err_t stack_err = stack_verify(&processor->stk);
    stack_dump(&processor->stk, stack_err);
    printf("================================\n");
    
    getchar();
    
    return NO_ERROR;
}
Processor_err processor_CALL(Processor *processor) {
    assert(processor != NULL);
    // адрес перехода (метка функции)
    int jump_address = processor->code[processor->counter + 1];
    
    
    int return_address = processor->counter + 2;// адрес возврата - следующая команда после CALL 

    Stack_err_t stack_err = stack_push(&processor->call_stack, return_address);
    if (stack_err != STACK_NO_ERROR) {
        printf("Ошибка стека вызовов при CALL\n");
        stack_dump(&processor->call_stack, stack_err);
        return STACK_ERROR;
    }
    
    processor->counter = jump_address;
    
    //printf("CALL: переход к адресу %d, адрес возврата %d\n", jump_address, return_address);
    
    return NO_ERROR;
}
Processor_err processor_RET(Processor *processor) {
    assert(processor != NULL);

    int return_address = 0;
    Stack_err_t stack_err = stack_pop(&processor->call_stack, &return_address);
    if (stack_err != STACK_NO_ERROR) {
        printf("Ошибка стека вызовов при RET\n");
        stack_dump(&processor->call_stack, stack_err);
        return STACK_ERROR;
    }
    
    processor->counter = return_address;
    
    //printf("RET: возврат к адресу %d\n", return_address);
    
    return NO_ERROR;
}




Processor_err processor_PUSHM(Processor *processor) { // PUSHM - из памяти в стек
    int reg_index = processor->code[processor->counter + 1];
        
    int address = processor->regs[reg_index].reg_val; // адрес из регистра
    if (address < 0 || address >= 100) {
        printf("Ошибка: неверный адрес памяти %d (из регистра)\n", address);
        return STACK_ERROR;
    } 

    type_t value = processor->RAM[address];
    stack_push(&processor->stk, value);
    
    return NO_ERROR;
}

Processor_err processor_POPM(Processor *processor) { // POPM - из стека в память
    int reg_index = processor->code[processor->counter + 1];
    
    int address = processor->regs[reg_index].reg_val; // адрес из регистра
    if (address < 0 || address >= 100) {
        printf("Ошибка: неверный адрес памяти %d (из регистра)\n", address);
        return STACK_ERROR;
    }
    
    type_t value = 0;
    stack_pop(&processor->stk, &value);
    processor->RAM[address] = value;
    
    return NO_ERROR;
}













Processor_err processor_destroy(Processor *processor) {
    assert(processor != NULL);
    
    Stack_err_t stack_err = stack_destroy(&processor->stk);
    if (stack_err != STACK_NO_ERROR) {
        printf("Ошибка уничтожения стека: %d\n", stack_err);
        return STACK_ERROR;
    }
    stack_err = stack_destroy(&processor->call_stack);
    if (stack_err != STACK_NO_ERROR) {
        printf("Ошибка уничтожения стека вызовов: %d\n", stack_err);
        return STACK_ERROR;
    }
    
    processor->counter = 0;
    memset(processor->regs, 0, sizeof(processor->regs)); 
    memset(processor->code, 0, sizeof(processor->code));
    
    return NO_ERROR;
}