#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <sys/mman.h>

#define COMMAND_SIZE 128
#define MEMORY_SIZE 128
#define NUM_REGISTERS 8
#define MAX_OPERATION_NAME 7

typedef struct CPU {
    int registers[NUM_REGISTERS];    
    int memory[MEMORY_SIZE];         
    int IP;                          
    int WSR;                         
} CPU;

void initialize(CPU *cpu);
void initialize_for_exit(CPU *cpu);
void add(CPU *cpu, int register1, int register2);
void sub(CPU *cpu, int register1, int register2);
void mov(CPU *cpu, int registers, int value);
void layo(CPU *cpu);
void load(CPU *cpu, int r, int mem_addr);
void store(CPU *cpu, int register1, int mem_addr);
void discard(CPU *cpu, int count);
void execute_instruction(CPU *cpu, char *command);
void help_function();

#endif
