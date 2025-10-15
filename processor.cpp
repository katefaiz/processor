
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

int main() { 
   
    Processor processor;
    Processor_err proc_err = processor_init(&processor, 100);
    if (proc_err != NO_ERROR) {
        printf("Ошибка инициализации процессора\n");
        return 1;
    }
    int program[40] = {}; 
    asm_commands_data(program);
    
    bite_code_file(program, 100);
    bite_code_read(&processor);
    run_bytecode(&processor);
    printf("-----------------------------------\n");
    stack_dump(&processor.stk, stack_verify(&processor.stk));
    for (int i = 0; i < 40; i++) {
        printf("%d ", processor.code[i]);
    }

    printf("\n------------регистры---------------\n");
    for (int i = 0; i < 4; i++) {
        printf("Регистр %d: имя=%d, значение=%d\n", i, processor.regs[i].reg, processor.regs[i].reg_val);
    }
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
    
    return NO_ERROR;
}

Processor_err run_bytecode(Processor *processor) { //выполняет байт-код
    assert(processor != 0);
    
    while (processor->counter < 100) {
        int command = processor->code[processor->counter];
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
                    step = 2;
                break; 
                default: 
                    printf("Неизвестная команда: %d\n", command);
                    stack_destroy(&processor->stk);
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
