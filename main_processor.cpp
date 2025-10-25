#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <sys/stat.h>

#include "processor.h"

int main() { 
   
    Processor processor;
    processor_init(&processor, 100);
    
    Assembler assembler; // TODO: это другая программа
    assembler_init(&assembler);
    assembler_compile(&assembler);
    assembler_save_to_file(&assembler);
    bite_code_read(&processor);
    run_bytecode(&processor);

    printf("-----------------------------------\n");
    // processor_dump(&processor);
    printf("МЕТКИ:\n");
    for (int i = 0; i < 20; i++) { 
        printf("%d ", assembler.labels[i]);
        
    }
    
    assembler_destroy(&assembler);
    processor_destroy(&processor);
}