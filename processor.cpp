
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "stack_funks.h"

void input_calculate(Stack_t *stk, char * command, type_t *value);
Commands comparing_commands(char *command);
void stack_calculate (Stack_t *stk, char *command, int value);

int main() { 
    Stack_t stk1 = {};
    type_t capasity = 10;
    Stack_err_t err = stack_init(&stk1, capasity);
    if (err != STACK_OK) {
        stack_output_err(err);
    }
    //stack_push(&stk1, 10);
    
    // type_t value_pop = 0; //сюда будет извлечен верхний эл-т стека
    // err = stack_pop(&stk1, &value_pop);

    // для калькулятора:
    type_t value = 0;
    char command[10] = "";
    input_calculate(&stk1, command, &value);

    stack_dump(&stk1, err);
    //--------------
    stack_destroy(&stk1);
    
    return 0;
}


void input_calculate(Stack_t *stk, char *command, type_t *value) {
    printf("Введите команду (или 'HLT' для выхода):\n");

    while (1) {
        
        if (scanf("%4s", command) != 1) { 
            printf("Ошибка чтения команды.\n");
            clear_enter(); 
            continue; 
        }

        if (comparing_commands(command) == HLT) {
    
            return; 
        }

        if (comparing_commands(command) == PUSH) {
            if (scanf("%d", value) != 1) {
                printf("Ошибка чтения значения для PUSH.\n");
                clear_enter();
                continue;
            }
        } 
        else {
            *value = 0; // Обнуляем value для не-PUSH команд
        }
        stack_calculate(stk, command, *value);
    }
    
    
}

Commands comparing_commands(char *command) {

    if (strcmp(command, "PUSH") == 0)   return PUSH;
    if (strcmp(command, "ADD") == 0)    return ADD;
    if (strcmp(command, "SUB") == 0)    return SUB;
    if (strcmp(command, "MUL") == 0)    return MUL;
    if (strcmp(command, "DIV") == 0)    return DIV;
    if (strcmp(command, "OUT") == 0)    return OUT;
    if (strcmp(command, "HLT") == 0)    return HLT;
    if (strcmp(command, "SQRT") == 0)   return SQRT;
    else                                return ERROR;
}

void stack_calculate (Stack_t *stk, char* command, int value) {

    switch (comparing_commands(command)) {
        case  PUSH: {
            stack_push(stk, value);
            break;
        }
        case  ADD: {
            type_t val1 = 0;
            stack_pop(stk, &val1);
            type_t val2 = 0;
            stack_pop(stk, &val2);
            stack_push(stk, val1 + val2);
            break;
        }
        case SUB: {
            type_t val1 = 0;
            stack_pop(stk, &val1);
            type_t val2 = 0;
            stack_pop(stk, &val2);
            stack_push(stk, val1 - val2);
            break;
        }
        case MUL: {
            type_t val1 = 0;
            stack_pop(stk, &val1);
            type_t val2 = 0;
            stack_pop(stk, &val2);
            stack_push(stk, val1 * val2);
            break;
        }   
        case DIV: {
            type_t val1 = 0;
            stack_pop(stk, &val1);
            type_t val2 = 0;
            stack_pop(stk, &val2);
            stack_push(stk, val1 / val2); 
            break;
        }
        case OUT: {
            type_t val = 0;
            stack_pop(stk, &val);
            printf("%d", val);
            break;
        }
        case HLT: 
            break;
        case SQRT: {
            type_t val = 0;
            stack_pop(stk, &val);
            val = sqrtf(val);
            stack_push(stk, val);
            break;
        }
        case ERROR:
            break;
        default:
            printf("Команда не распознана, введите еще раз\n");
    }
}
