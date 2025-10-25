
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <sys/stat.h>

#include "commands_funks.h" 
#include "assembler.h"
#include "assembler_struct_enum.h"


Assembler_err assembler_init(Assembler *assembler) {
    assert(assembler != NULL);
    
    memset(assembler->program, 0, sizeof(assembler->program));
    memset(assembler->labels, -1, sizeof(assembler->labels)); 
    assembler->labels_count = 0;
    assembler->program_size = 0;
    
    return ASS_NO_ERROR;
}


Assembler_err assembler_compile(Assembler *assembler) {
    assert(assembler != NULL);

    FILE * filestream = fopen(assembler->source_file, "r");
    if (filestream == NULL) 
        return ASS_OPENFILE_ERROR;
    
    char line[100];
    int count = 0;//сюда будем считать размер байт-кода

    // ПЕРВЫЙ ПРОХОД: 
    while(fgets(line, sizeof(line), filestream)) {
        int ind = 0;
        while (line[ind] == ' ') // TODO: skip_space()
            ind++;

        if (line[ind] == ';') {
            continue;
        }
        
        if (line[ind] == ':') {
            ind++;
            int label_index = atoi(line + ind);
            
            if (label_index >= 0 && label_index < 20) { 
                assembler->labels[label_index] = count;
                //printf("Метка :%d указывает на адрес %d\n", label_index, count);
            }
        }
        else if (line[ind] != '\n' && line[ind] != '\0') { 
            char command[10] = {};
            int cmd_start = ind;
            
            while (line[ind] != ' ' && line[ind] != '\0' && line[ind] != '\n') {
                if (ind - cmd_start < 9) {
                    command[ind - cmd_start] = line[ind]; // TODO: можно всю строку. Вспомни строковые функции
                }
                ind++;
            }
            command[ind - cmd_start] = '\0';
            
            Commands cmd_code = comparing_commands(command);
            
            //считаем сколько ячеек занимает команда
            if (cmd_code == PUSH || cmd_code == POPR || cmd_code == PUSHR || 
                cmd_code == JMP || cmd_code == CALL || cmd_code == POPM || cmd_code == PUSHM) {
                count += 2; 
            }
            else if (cmd_code != ERROR) {
                count += 1; 
            }
        }
    }
    
    fseek(filestream, 0, SEEK_SET); //прыг на начало файла
    count = 0;
    
    // ВТОРОЙ ПРОХОД: компиляция байт-кода
    while(fgets(line, sizeof(line), filestream)) {
        char command[10] = {};
        char value_str[50] = {};
        int value = 0;
        int ind = 0;

        while (line[ind] == ' ')
            ind++;

        if (line[ind] == ':') {
            // Пропускаем метку и ее номер
            ind++;
            while (line[ind] >= '0' && line[ind] <= '9')
                ind++;
            // Пропускаем пробелы после метки
            while (line[ind] == ' ')
                ind++;
            
            // Если после метки есть команды - обрабатываем их
            if (line[ind] != '\n' && line[ind] != '\0' && line[ind] != ';') {
                // Продолжаем обработку с этого места
               
            } else {
                // Если после метки только комментарий или пусто - пропускаем строку
                continue;
            }
        }
        if (line[ind] == ';') {
            continue;
        }
        
        //пропускаем пустые строки
        if (line[ind] == '\n' || line[ind] == '\0') {
            continue;
        }

        int cmd_start = ind;
        while (line[ind] != ' ' && line[ind] != '\0' && line[ind] != '\n') {
            if (ind - cmd_start < 9) {
                command[ind - cmd_start] = line[ind];
            }
            ind++;
        }
        command[ind - cmd_start] = '\0';

        Commands cmd_code = comparing_commands(command);
        
        while (line[ind] == ' ')
            ind++;

        if (cmd_code == PUSH) {
            if (line[ind] != '\0' && line[ind] != '\n') {
                value = atoi(line + ind);
                assembler->program[count++] = cmd_code;
                assembler->program[count++] = value;
                //printf("PUSH: команда=%d, значение=%d\n", cmd_code, value);
            }    
        }
        else if (cmd_code == PUSHR || cmd_code == POPR || cmd_code == POPM || cmd_code == PUSHM) { 
            if (line[ind] != '\0' && line[ind] != '\n') {
                int value_ind = 0;
                while (line[ind] != '\0' && line[ind] != '\n' && line[ind] != ' ') {
                    if (value_ind < 50) {
                        value_str[value_ind] = line[ind];
                        value_ind++;
                    }
                    ind++;
                }
                value_str[value_ind] = '\0';
                
                Registers_name reg = comparing_registers(value_str);
                assembler->program[count++] = cmd_code;
                assembler->program[count++] = (int)reg;
                //printf("%s: команда=%d, регистр='%s'->%d\n", command, cmd_code, value_str, (int)reg);
            }
        }
        else if (cmd_code == JMP || cmd_code == CALL) {  
            if (line[ind] != '\0' && line[ind] != '\n') {
                if (line[ind] == ':')  
                    ind++;
                    
                value = atoi(line + ind);
                assembler->program[count++] = cmd_code;
                assembler->program[count++] = value;
                //printf("%s: команда=%d, метка=%d\n", command, cmd_code, value);
            }
        }
        else if (cmd_code != ERROR) {
            assembler->program[count++] = cmd_code;
            //printf("%s: команда=%d\n", command, cmd_code);
        }
    }
    
    assembler->program_size = count;
    fclose(filestream);
    assembler_resolve_labels(assembler);
    return ASS_NO_ERROR;
}

Assembler_err assembler_resolve_labels(Assembler *assembler) { //из индекса метки получаю реальные адреса
    assert(assembler != NULL);
    
    for (int i = 0; i < assembler->program_size; i++) {
        if (assembler->program[i] == JMP || assembler->program[i] == CALL) {
            int label_index = assembler->program[i + 1]; //индекс метки в массиве меток (для :2 это 2)
            
            int address = assembler->labels[label_index];//адрес, куда указывает метка
            assembler->program[i + 1] = address;            
            i++; 
        }
    }
    return ASS_NO_ERROR;
}

Assembler_err assembler_save_to_file(Assembler *assembler) { // записывает массив с кодом в файл
    FILE *filestream = fopen(assembler->output_file, "w");
    if (filestream == NULL) {
        printf("Ошибка открытия файла \n");
        return ASS_OPENFILE_ERROR;
    }
    
    for (int i = 0; i < assembler->program_size; i++) {
        fprintf(filestream, "%d ", assembler->program[i]);
    }
    
    fclose(filestream);
    return ASS_NO_ERROR;

}

Assembler_err assembler_destroy(Assembler *assembler) {
    assert(assembler != NULL);
    
    memset(assembler->program, 0, sizeof(assembler->program));
    memset(assembler->labels, -1, sizeof(assembler->labels));
    assembler->program_size = 0;
    
    return ASS_NO_ERROR;
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
    if (strcmp(command, "CALL") == 0)   
        return CALL;   
    if (strcmp(command, "RET") == 0)    
        return RET;
    if (strcmp(command, "POPM") == 0)   
        return POPM;
    if (strcmp(command, "PUSHM") == 0)   
        return PUSHM;
    if (strcmp(command, "DRAW") == 0)   
        return DRAW;
    else                                
        return ERROR;
}

Registers_name comparing_registers(const char *reg_name) {

    if (strcmp(reg_name, "RAX") == 0) 
        return RAX;
    if (strcmp(reg_name, "RBX") == 0) 
        return RBX;
    if (strcmp(reg_name, "RCX") == 0)     
        return RCX;
    if (strcmp(reg_name, "RDX") == 0) 
        return RDX;
    
    printf("Ошибка: неизвестный регистр '%s'\n", reg_name);
    return ROX; 
}




Assembler_err disasm_commands_data(int program[]) { // проверка ассемблирования
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
    return ASS_NO_ERROR;
}


















// Processor_err asm_commands_data(int *program) { //считывает файл с командами и записывает байт-код в файл 

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
















//по факту дальше уже не нужно 

// void stack_calculate (Stack_t *stk, char* command, int value) { 
    
//     switch (comparing_commands(command)) {
//         case  PUSH: {
//             //int value = 0;
//             stack_push(stk, value);
//             break;
//         }
//         case  ADD: {
//             type_t val1 = 0;
//             stack_pop(stk, &val1);
//             type_t val2 = 0;
//             stack_pop(stk, &val2);
//             stack_push(stk, val1 + val2);
//             break;
//         }
//         case SUB: {
//             type_t val1 = 0;
//             stack_pop(stk, &val1);
//             type_t val2 = 0;
//             stack_pop(stk, &val2);
//             stack_push(stk, val1 - val2);
//             break;
//         }
//         case MUL: {
//             type_t val1 = 0;
//             stack_pop(stk, &val1);
//             type_t val2 = 0;
//             stack_pop(stk, &val2);
//             stack_push(stk, val1 * val2);
//             break;
//         }   
//         case DIV: {
//             type_t val1 = 0;
//             stack_pop(stk, &val1);
//             type_t val2 = 0;
//             stack_pop(stk, &val2);
//             stack_push(stk, val2 / val1); 
//             break;
//         }
//         case OUT: {
//             type_t val = 0;
//             stack_pop(stk, &val);
//             printf("%d\n", val);
//             break;
//         }
//         case HLT: 
//         break;
//         case SQRT: {
//             type_t val = 0;
//             stack_pop(stk, &val);
//             val = sqrtf(val);
//             stack_push(stk, val);
//             break;
//         }
//         case ERROR:
//         break;
//         default:
//         printf("Команда не распознана\n");
//     }
// }



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