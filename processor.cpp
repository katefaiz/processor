
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <sys/stat.h>

#include "commands_funks.h" 
#include "assembler.h"

Processor_err bite_code_read(Processor *processor);
Processor_err run_bytecode(Processor *processor);
Processor_err processor_init(Processor *processor, type_t capacity);
Processor_err processor_dump(Processor *processor);


int main() { 
   
    Processor processor;
    Processor_err proc_err = processor_init(&processor, 100);
    if (proc_err != NO_ERROR) {
        printf("Ошибка инициализации процессора\n");
        return 1;
    }
    
    Assembler assembler;
    proc_err = assembler_init(&assembler, "commands_data.txt", "bite_code.txt");
    if (proc_err != NO_ERROR) {
        printf("Ошибка инициализации асемблера\n");
        return 1;
    }

    proc_err = assembler_compile(&assembler);
    if (proc_err != NO_ERROR) {
        printf("Ошибка компиляции\n");
        return 1;
    }
    //assembler_compile(&assembler);
    proc_err = assembler_save_to_file(&assembler);
    if (proc_err != NO_ERROR) {
        printf("Ошибка сохранения байт-кода\n");
        return 1;
    }



    bite_code_read(&processor);
    run_bytecode(&processor);

    printf("-----------------------------------\n");
    //stack_dump(&processor.stk, stack_verify(&processor.stk));
    processor_dump(&processor);
    printf("МЕТКИ:\n");
    for (int i = 0; i < 20; i++) { 
        printf("%d ", assembler.labels[i]);
        
    }
    
    assembler_destroy(&assembler);
    processor_destroy(&processor);
}

Processor_err processor_init(Processor *processor, type_t capacity) {
    assert(processor != NULL);
    
    processor->counter = 0;

    processor->regs[0] = {RAX, 0};
    processor->regs[1] = {RBX, 0};
    processor->regs[2] = {RCX, 0};
    processor->regs[3] = {RDX, 0};
    
    memset(processor->code, 0, sizeof(processor->code));
    
    Stack_err_t stack_err = stack_init(&processor->stk, capacity);
    if (stack_err != STACK_NO_ERROR) {
        printf("Ошибка инициализации стека: %d\n", stack_err);
        return STACK_ERROR;
    }

    stack_err = stack_init(&processor->call_stack, 20); // стек на 20 адресов возврата
    if (stack_err != STACK_NO_ERROR) {
        printf("Ошибка инициализации стека вызовов: %d\n", stack_err);
        stack_destroy(&processor->stk);
        return STACK_ERROR;
    }

    return NO_ERROR;
}

Processor_err run_bytecode(Processor *processor) { //выполняет байт-код
    assert(processor != 0);
    
    while (processor->counter < 100) {
        int command = processor->code[processor->counter];
        printf("Выполняется команда %d по адресу %d\n", command, processor->counter);
        if (command == HLT) 
            break;
        
            int step = 1;
            switch (command) {
                case HLT: 
                //processor_HLT(processor);
                    break;
                case PUSH: 
                    processor_PUSH(processor);
                    step = 2;
                    break;          
                case ADD: 
                    processor_ADD(processor);
                    break;
                case SUB: 
                    processor_SUB(processor);
                    break;
                case MUL: 
                    processor_MUL(processor);
                    break;
                case DIV: 
                    processor_DIV(processor);
                    break;
                case SQRT: 
                    processor_SQRT(processor);
                    break;
                case OUT: 
                    processor_OUT(processor);
                    break;
                case PUSHR: 
                    processor_PUSHR(processor);
                    step = 2;
                    break;
                case POPR: 
                    processor_POPR(processor);
                    step = 2;
                    break;  
                case JMP: 
                    processor_JMP(processor);  
                    continue;
                case CALL:                  
                    processor_CALL(processor);
                    continue;
                case RET:                     
                    processor_RET(processor);
                    continue;
                default: 
                    printf("Неизвестная команда: %d\n", command);
                    return STACK_ERROR;
                
            }
        processor->counter += step;
    }
    return NO_ERROR;
}

Processor_err bite_code_read(Processor *processor) { //читает файл с байт-кодом и записывает в массив 
    assert(processor != NULL);
    
    FILE* filesteam = fopen("bite_code.txt", "r");
    if (filesteam == NULL) {
        printf("Ошибка: не удалось открыть файл\n");
        return OPENFILE_ERROR;
    }
    
    int count = 0;
    while (count < 100 && fscanf(filesteam, "%d", &processor->code[count]) == 1) {
        count++;
        if (processor->code[count - 1] == 0)
            break;
            
    }
    
    fclose(filesteam);
    return NO_ERROR;

}

Processor_err processor_dump(Processor *processor) {
    assert(processor != NULL);
    
    printf("\n=== СОСТОЯНИЕ ПРОЦЕССОРА ===\n");
    
    printf(" РЕГИСТРЫ:\n");
    const char* reg_names[] = {"ROX", "RAX", "RBX", "RCX", "RDX"};
    for (int i = 0; i < 4; i++) {
        printf("  %s = %d\n", reg_names[i], processor->regs[i].reg_val);
    }
    
    printf(" СЧЕТЧИК КОМАНД: %d\n", processor->counter);
    printf("\n");
    printf("\n СТЕК ВЫЗОВОВ\n");
    if (processor->call_stack.size == 0) {
        printf("Стек вызовов пуст\n");
    } 
    else {
        for (int i = 0; i < processor->call_stack.size; i++) {
            printf("[%d] = %d (адрес возврата)\n", i, processor->call_stack.data[i]);
        }
    }
    
    Stack_err_t stack_err = stack_verify(&processor->stk);
    printf("\n ОСНОВНОЙ СТЕК \n");
    stack_dump(&processor->stk, stack_err);
    
    printf("================================\n\n");
    
    return NO_ERROR;
}