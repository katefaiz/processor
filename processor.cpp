
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <sys/stat.h>

#include "commands_funks.h" 



void input_calculate(Stack_t *stk, char * command, type_t *value);
Commands comparing_commands(const char *command);
void stack_calculate (Stack_t *stk, char *command, int value);
Processor_err asm_commands_data(int *program);
Processor_err disasm_commands_data(int program[]);
Processor_err bite_code_file(int program[], int size);
Processor_err bite_code_read(Processor *processor);
Processor_err run_bytecode(Processor *processor);
Processor_err processor_init(Processor *processor, type_t capacity);
Registers_name comparing_registers(const char *reg_name);



int main() { 
    // Stack_t stk1 = {};
    // type_t capasity = 10;
    // Stack_err_t err = stack_init(&stk1, capasity);
    // if (err != NO_ERROR) {
    //     stack_output_err(err);
    // }

    Processor processor;
    Processor_err proc_err = processor_init(&processor, 100);
    if (proc_err != NO_ERROR) {
        printf("Ошибка инициализации процессора\n");
        return 1;
    }
    int program[40] = {}; 
    asm_commands_data(program);
    //disasm_commands_data(program);
    // для калькулятора:
    // type_t value = 0;
    // char command[10] = "";
    // input_calculate(&stk1, command, &value);

    // stack_dump(&stk1, err);
    //--------------
    // stack_destroy(&stk1);
    bite_code_file(program, 100);
    bite_code_read(&processor);
    run_bytecode(&processor);
    printf("-----------------------------------\n");
    stack_dump(&processor.stk, stack_verify(&processor.stk));
    for (int i = 0; i < 40; i++) {
        printf("%d ", processor.code[i]);
    }
    printf("-------регистры---------------\n");
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
        //processor_HLT(processor);
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



//------------------------------------------------------------------------------------------------------------------

Processor_err asm_commands_data(int *program) {
    assert(program != NULL);

    FILE * filestream = fopen("commands_data.txt", "r");
    if (filestream == NULL) 
        return OPENFILE_ERROR;
    
    char line[100];
    int count = 0;

    while(fgets(line, sizeof(line), filestream)) {
        char command[10] = {};
        char value_str[50] = {};
        int value = 0;
        int ind = 0;

        // Читаем команду до первого пробела или конца строки
        while (line[ind] != ' ' && line[ind] != '\0' && line[ind] != '\n') {
            if (ind < 9) {
                command[ind] = line[ind];
            }
            ind++;
        }
        command[ind] = '\0';

        Commands cmd_code = comparing_commands(command);

        if (cmd_code == PUSH) {
            // Пропускаем пробелы
            while (line[ind] == ' ' || line[ind] == '\t') ind++;
            
            if (line[ind] != '\0' && line[ind] != '\n') {
                value = atoi(line + ind);
                program[count++] = cmd_code;  // команда PUSH
                program[count++] = value;     // значение
            }
            printf("  PUSH: команда=%d, значение=%d\n", cmd_code, value);
        }
        else if (cmd_code == PUSHR || cmd_code == POPR) {
            // Пропускаем пробелы
            while (line[ind] == ' ' || line[ind] == '\t') ind++;
            
            if (line[ind] != '\0' && line[ind] != '\n') {
                int value_ind = 0;
                // Читаем имя регистра
                while (line[ind] != '\0' && line[ind] != '\n' && line[ind] != ' ' && line[ind] != '\t') {
                    if (value_ind < 49) {
                        value_str[value_ind] = line[ind];
                        value_ind++;
                    }
                    ind++;
                }
                value_str[value_ind] = '\0';
                
                Registers_name reg = comparing_registers(value_str);
                program[count++] = cmd_code;      // команда 
                program[count++] = (int)reg;      // номер регистра
                printf("  %s: команда=%d, регистр='%s'->%d\n", command, cmd_code, value_str, (int)reg);
            }
        }
        else if (cmd_code == JMP) {  
            while (line[ind] == ' ' || line[ind] == '\t') ind++;
            
            if (line[ind] != '\0' && line[ind] != '\n') {
                value = atoi(line + ind);
                program[count++] = cmd_code;  
                program[count++] = value;     
            }
            printf("  JMP: команда=%d, адрес=%d\n", cmd_code, value);
        }
        else {
            // Команды без аргументов
            program[count++] = cmd_code;
        }
    }
    
    fclose(filestream);
    return NO_ERROR;
}


// Processor_err asm_commands_data(int *program) { //считывает файл с командами и записывает байт-код в файл 
// //TODO: ЗДЕСЬ НАДО ПОСМОТРЕТЬ
//     assert(program != NULL);

//     FILE * filestream = fopen("commands_data.txt", "r");
//     if (filestream == NULL) 
//     return OPENFILE_ERROR;
    
    
//     char line[100]; // каждая отдельная строка
//     int count = 0; //счетчик для массива

//     while(fgets(line, sizeof(line), filestream)) {
//         char command[10] = {};
//         char value_str[50] = {};
//         int value = 0;
//         int ind = 0;
//         //printf("Считана строка: '%s'", line);

//         while (line[ind] != ' ' && line[ind] != '\0' && line[ind] != '\n'){
//             command[ind] = line[ind]; //копирую команду
//             ind++;
//         }
//         command[ind] = '\0';

//         Commands cmd_code = comparing_commands(command);
//         //printf("\n%d", cmd_code);
//         if (cmd_code == PUSH) {
//             // Пропускаем пробелы после команды
//             while (line[ind] == ' ' || line[ind] == '\t') 
//                 ind++;
            
//             if (line[ind] != '\0' && line[ind] != '\n') {
//                 value = atoi(line + ind);
//                 program[count++] = cmd_code;  
//                 program[count++] = value;     
//                 printf("  PUSH: команда=%d, значение=%d\n", cmd_code, value);
//             }
//         }
        
//         else if (cmd_code == PUSHR || cmd_code == POPR) {
            
//             while (line[ind] == ' ' || line[ind] == '\t') //пропускаю пробелы после команды
//                 ind++;
            
//             if (line[ind] != '\0' && line[ind] != '\n') {
//                 int value_ind = 0;
//                 while (line[ind] != '\0' && line[ind] != '\n' && line[ind] != ' ' && line[ind] != '\t') {
//                     if (value_ind < 49) {
//                         value_str[value_ind] = line[ind];
//                         value_ind++;
//                     }
//                     ind++;
//                 }
//                 value_str[value_ind] = '\0';
                
//                 Registers_name reg = comparing_registers(value_str);
//                 program[count++] = cmd_code;  
//                 program[count++] = (int)reg;  
//                 printf("  %s: команда=%d, регистр='%s'->%d\n", 
//                     command, cmd_code, value_str, (int)reg);
//             }
//         }
//         else {
//             program[count++] = cmd_code;
//            // printf("  %s: команда=%d (без аргументов)\n", command, cmd_code);
//         }
 
//     }
//     fclose(filestream);
//     return NO_ERROR;
// }
Processor_err bite_code_file(int program[], int size) { // записывает массив с кодом в файл
    FILE *filestream = fopen("bite_code.txt", "w");
    if (filestream == NULL) {
        printf("Ошибка открытия файла \n");
        return OPENFILE_ERROR;
    }
    
    int i = 0;
    while (i < size && program[i] != 0) {
        fprintf(filestream, "%d ", program[i]);
        i++;
    }
    if (i < size) {
        fprintf(filestream, "%d", program[i]);
    }
    
    fclose(filestream);
    return NO_ERROR;

}



Processor_err disasm_commands_data(int program[]) { // проверка ассемблирования
    int index = 0;
    while (program[index] != HLT) {
        switch (program[index]) {
            case 0:
                printf("HLT\n");
                break;
            case 1:
            printf("PUSH %d\n", program[index + 1]);
                index += 2;
                break;
            case 2:
                printf("ADD\n");
                index++;
                break;
            case 3:
                printf("SUB\n");
                index++;
                break;
            case 4:
                printf("MUL\n");
                index++;
                break;
            case 5:
                printf("DIV\n");
                index++;
                break;
            case 6:
                printf("OUT\n");
                index++;
                break;
            case 7:
                printf("SQRT\n");
                index++;
                break;
            case 42:
                printf("POPR\n");
                index+=2;
                break;
            case 33:
                printf("PUSHR\n");
                index+=2;
                break;
            
            case 666:
                printf("ERROR\n");
                index++;
                break;
            default:
                printf("ERROR!!!\n");          
        }      
    }
    return NO_ERROR;
}


Commands comparing_commands(const char *command) { 

    if (strcmp(command, "PUSH") == 0)   
    return PUSH;
    if (strcmp(command, "ADD") == 0)    
    return ADD;
    if (strcmp(command, "SUB") == 0)    
    return SUB;
    if (strcmp(command, "MUL") == 0)    
    return MUL;
    if (strcmp(command, "DIV") == 0)    
    return DIV;
    if (strcmp(command, "OUT") == 0)    
    return OUT;
    if (strcmp(command, "HLT") == 0)    
        return HLT;
    if (strcmp(command, "SQRT") == 0)   
        return SQRT;
    if (strcmp(command, "POPR") == 0)   
        return POPR;
    if (strcmp(command, "PUSHR") == 0)   
        return PUSHR;
    if (strcmp(command, "JMP") == 0)    
        return JMP;
    else                                
    return ERROR;
}

Registers_name comparing_registers(const char *reg_name) {
    // Убедитесь, что регистры правильно сравниваются
    if (strcmp(reg_name, "RAX") == 0) 
        return RAX;
    if (strcmp(reg_name, "RBX") == 0) 
        return RBX;
    if (strcmp(reg_name, "RCX") == 0)     
        return RCX;
    if (strcmp(reg_name, "RDX") == 0) 
        return RDX;
    
    printf("Ошибка: неизвестный регистр '%s'\n", reg_name);
    return ROX; // Возвращаем RAX по умолчанию при ошибке
}





















//по факту дальше уже не нужно 

void stack_calculate (Stack_t *stk, char* command, int value) { 
    
    switch (comparing_commands(command)) {
        case  PUSH: {
            //int value = 0;
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
            stack_push(stk, val2 / val1); 
            break;
        }
        case OUT: {
            type_t val = 0;
            stack_pop(stk, &val);
            printf("%d\n", val);
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
        printf("Команда не распознана\n");
    }
}



// Stack_err_t run_commands_data(int program[]) {
//     Stack_t *stk;

//     for (int i = 0; program[i] != 0; i++) {
//         Commands command = comparing_commands(program[i]);
//         if (program[i] == 1) {
//             stack_calculate(stk, program[i], program[i + 1]);
//             i++;
//         }

//     }


//     return NO_ERROR;
// }



// void input_calculate(Stack_t *stk, char *command, type_t *value) {
//     printf("Введите команду (или 'HLT' для выхода):\n");

//     while (1) {
        
//         if (scanf("%4s", command) != 1) { 
//             printf("Ошибка чтения команды.\n");
//             clear_enter(); 
//             continue; 
//         }

//         if (comparing_commands(command) == HLT) {
    
//             return; 
//         }

//         if (comparing_commands(command) == PUSH) {
//             if (scanf("%d", value) != 1) {
//                 printf("Ошибка чтения значения для PUSH.\n");
//                 clear_enter();
//                 continue;
//             }
//         } 
//         else {
//             *value = 0; // Обнуляем value для не-PUSH команд
//         }
//         stack_calculate(stk, command, *value);
//     } 
// }