
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "stack_check.h"

Stack_err_t stack_dump(Stack_t *stk, Stack_err_t err) {
    assert(stk != NULL);

    stack_output_err(err);
    printf("\n=== STACK DUMP ===\n");
    printf("addres\t= %p\n", stk);
    printf("size\t= %zd\n", stk -> size);
    printf("capasity= %zd\n", stk -> capasity);
    for (ssize_t i = 0; i < stk -> size; i++){
        printf("*[%zd] = %d\n", i, stk -> data[i]);
    }
    for (ssize_t i = stk -> size; i < stk -> capasity; i++) {
        printf("[%zd] = %d (poizon)\n", i, stk -> data[i]);
    }
    
    return STACK_NO_ERROR;
}
Stack_err_t stack_verify(Stack_t * stk) {
    assert(stk != NULL);
    
    if (stk == NULL)
        return STACK_INVALID_PTR;
    if ((stk -> left_canary != CANARY_VALUE) || (stk -> right_canary != CANARY_VALUE ))
        return CANARY_CORRUPTED; 
    if (stk -> data == NULL)
        return STACK_MEMORY_ERROR;
    if (stk -> capasity <= 0) 
        return STACK_INVALID_CAPACITY;
    if (stk -> size < 0) 
        return STACK_INVALID_SIZE; 
    if (stk -> size > stk -> capasity) 
        return STACK_CORRUPTED;
    for (long int i = stk -> size; i < stk -> capasity; i++){
        if (stk -> data[i] != POIZON)
            return STACK_POIZON_ERROR;
    }
    return STACK_NO_ERROR;
}

void stack_output_err(Stack_err_t err) {
    printf("\n");
    switch (err) {
        case STACK_NO_ERROR:
            printf("все гуд\n"); 
            break;
        case STACK_MEMORY_ERROR:
            printf("ошибка выделения памяти\n");
            break;
        case STACK_INVALID_PTR:
            printf("неверный указатель\n");
            break;
        case STACK_INVALID_CAPACITY:
            printf("ошибка в размере стека\n");
            break;
        case STACK_INVALID_SIZE:
            printf("ошибка в кол-ве элементов стека\n");
            break;
        case STACK_CORRUPTED:
            printf("ошибка в структуре стека, количество элементов больше, чем допустимый размер\n");
            break;
        case CANARY_CORRUPTED:
            printf("повреждена  канарейка\n");
            break;
        case STACK_POIZON_ERROR:
            printf("Ошибка в заполнении пустот значениями poizon\n");
            break;
        case STACK_OPENFILE_ERROR:
            printf("Ошибка в открытии файла\n");
            break;
        default:
            printf("ошибка возвращаемого значения");
    }
    return;
}

void stack_fill_poizon(Stack_t *stk) {
    assert(stk != NULL);

    for (long int i = stk -> size; i < stk -> capasity; i++) { 
        stk -> data[i] = POIZON;
    }
}

void clear_enter (void) { 
    int ch = NAN;
    while ((ch = getchar( )) != EOF && ch  != '\n')
        ;
}