#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
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
int flag = 0;

void initialize(CPU *cpu) {
    /*if (madvise(cpu->memory, MEMORY_SIZE * sizeof(int), MADV_SEQUENTIAL) != 0 || madvise(cpu->registers, NUM_REGISTERS * sizeof(int), MADV_SEQUENTIAL) != 0 ) {
        printf("Madvise error on memory or register\n");
    }*/
    // himnakanum chi ashxatum
    memset(cpu->registers, 0, NUM_REGISTERS * sizeof(int));
    memset(cpu->memory, 0, MEMORY_SIZE * sizeof(int));
    cpu->IP = 1;
    cpu->WSR = 1;
    flag = 1;
    printf("WSR: %d, IP: %d\n",cpu->WSR,cpu->IP);
};
void initialize_for_exit(CPU *cpu) {
    /*if (madvise(cpu->memory, MEMORY_SIZE * sizeof(int), MADV_SEQUENTIAL) != 0 || madvise(cpu->registers, NUM_REGISTERS * sizeof(int), MADV_SEQUENTIAL) != 0 ) {
        printf("Madvise error on memory or register\n");
        exit(EXIT_FAILURE);
    }*/
    // himnakanum chi ashxatum
    memset(cpu->registers, 0, NUM_REGISTERS);
    memset(cpu->memory, 0, MEMORY_SIZE);
    cpu->WSR = 0;
    flag = 0;
    printf("WSR: %d, IP: %d\n",cpu->WSR,cpu->IP);
};
void add(CPU *cpu,int register1,int register2) {
    if(register1 >= 0 && register1 <= NUM_REGISTERS  && register2 >= 0 & register2 <= NUM_REGISTERS) {
        cpu->registers[register1] += cpu->registers[register2];
        cpu->IP++;
        printf("R%d: %d, IP: %d\n",register1, cpu->registers[register1],cpu->IP);
    }else {
        printf("Invalid register range%d - %d\n", 0, NUM_REGISTERS - 1);
    }
}
void sub(CPU *cpu,int register1,int register2) {
     if(register1 >= 0 && register1 <= NUM_REGISTERS  && register2 >= 0 & register2 <= NUM_REGISTERS) {
        cpu->registers[register1] -= cpu->registers[register2];
        cpu->IP++;
        printf("R%d: %d, IP: %d",register1, cpu->registers[register1],cpu->IP);
    }else {
        printf("Invalid register range%d - %d\n", 0, NUM_REGISTERS - 1);
    }
}
void mov(CPU *cpu,int registers,int value) {
    if(registers >= 0 && registers <= NUM_REGISTERS) {
        cpu->registers[registers] = value;
        cpu->IP++;
        printf("R%d: %d, IP: %d\n",registers, cpu->registers[registers],cpu->IP);
    }else if(registers < 0 || registers > NUM_REGISTERS) {
        printf("Invalid register number\n");
    }else if(value < INT_MIN || value > INT_MAX){
        printf("Too larger number: Range %d - %d\n", INT_MIN, INT_MAX);
    }
}
void layo(CPU *cpu) {
    printf("Registers: ");
    /*if (madvise(cpu->registers, NUM_REGISTERS * sizeof(int), MADV_SEQUENTIAL) != 0) {
        printf("Madvise error on registers");
        exit(EXIT_FAILURE);
    }*/
   //himnakanum chi ashxatum
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("R%d: %d, ", i, cpu->registers[i]);
    }
    printf("WSR: %d, IP: %d\n",cpu->WSR,cpu->IP);
    printf("Memory: [");
    /*if (madvise(cpu->memory, MEMORY_SIZE * sizeof(int), MADV_SEQUENTIAL) != 0) {
        perror("Madvise error on memory");
        exit(EXIT_FAILURE);
    }*/
   // himnakanum chi ashxatum
    for(int i = 0; i < MEMORY_SIZE; i++) {
        printf("%d,",cpu->memory[i]);
    }
    printf("]\n");
}
void load(CPU *cpu, int r, int mem_addr) {
    if (mem_addr >= 0 && mem_addr < MEMORY_SIZE) {
        cpu->registers[r] = cpu->memory[mem_addr];
    } else {
        printf("Invalid memory address\n");
    }
}
void store(CPU *cpu, int register1, int mem_addr) {
    if (mem_addr >= 0 && mem_addr < MEMORY_SIZE) {
        cpu->memory[mem_addr] = cpu->registers[register1];
    } else {
        printf("Invalid memory address\n");
    }
}
void discard(CPU *cpu, int count) {
    int value = cpu->IP - count;
    if(cpu->IP <= 0 || cpu->IP == INT_MAX || value < 0 || value > INT_MAX) {
        printf("Cant change the IP value\n");
    }else {
        cpu->IP -= count;
    }
}
void execute_instruction(CPU *cpu,char *command) {
   char operation[MAX_OPERATION_NAME];
   memset(operation,0,sizeof(operation));
   int register1 = 0, register2 = 0;
   int value = 0,count = 0;

    if (sscanf(command, "%s R%d, R%d", operation, &register1,&register2) == 3) {
        if (strncmp(operation, "ADD", 3) == 0 && flag == 1) {
            add(cpu,register1, register2);
        }
        else if (strncmp(operation, "SUB", 3) == 0 && flag == 1) {
            sub(cpu,register1, register2);
        } else if(flag == 1) {
            printf("Invalid instruction format. Use 'HELP' for more information.\n");
        }
        else {
            printf("You need to start 'START'\n");
        }
    }
    else if (sscanf(command, "%s R%d, %d", operation, &register1, &value) == 3) {
        if (strncmp(operation, "MOV", 3) == 0 && flag == 1) {
            mov(cpu,register1, value);
        }else if (strncmp(operation, "LOAD", 4) == 0 && flag == 1) {
            load(cpu,register1, value);
        }else if (strncmp(operation, "STORE", 5) == 0 && flag == 1) {
            store(cpu,register1, value);
        }else if(flag == 1){
           printf("Invalid instruction format use 'HELP' for know more about instructions\n");
        }else {
            printf("You need to start 'START'\n");
        }
    }else if (sscanf(command, "%s %d", operation, &count) == 2) {
        if (strncmp(operation, "DISC", 4) == 0 && flag == 1) {
            discard(cpu, count);
        } else if(flag == 1) {
            printf("Invalid instruction format. Use 'HELP' for more information.\n");
        }
        else {
            printf("You need to start 'START'\n");
        }
    }else if (sscanf(command, "%s", operation) == 1) {
        if (strncmp(operation, "START", 5) == 0) {
            initialize(cpu);
        } else if (strncmp(operation, "EXIT", 4) == 0) {
            if (cpu->WSR == 1) {
                initialize_for_exit(cpu);
            } else {
                printf("Exiting simulation.\n");
                exit(EXIT_SUCCESS);
            }
        }else if(strncmp(operation, "LAYO",4) == 0 && flag == 1) {
            layo(cpu);
        } else if(flag == 1) {
            printf("Invalid instruction format. Use 'HELP' for more information.\n");
        }
        else {
            printf("You need to start 'START'\n");
        }
    }
}

void help_function() {
    printf("Instructions Set:\n ADD Rdest, Rsrc1, Rsrc2: Adds the values in Rsrc1 and Rsrc2, stores the result in Rdest. Rsrc2 can be literal value.\n");
    printf("SUB Rdest, Rsrc1, Rsrc2: Subtracts the value in Rsrc2 from Rsrc1, stores the result in Rdest. Rsrc2 can be literal value\n");
    printf("MOV Rdest, value: Moves the immediate value into Rdest. Value can be register.\n");
    printf("LOAD Rdest, address: Loads the value from address in memory into Rdest.\n");
    printf("STORE Rsrc, address: Stores the value in Rsrc into address in memory.");
    printf("START: Sets WSR to 1, initializes CPU, resets memory.\n");
    printf("EXIT: Sets WSR to 0; simulator terminates if executed twice consecutively.\n");
    printf("DISC N: Discards the effects of the previous N instructions.\n");
    printf("LAYO: Outputs the current state of all registers and memory.\n");
}
int main () {
    CPU cpu;

    char command[COMMAND_SIZE];
    char memory[MEMORY_SIZE];

    printf("CPU Simulator\n");

    while(1) {
        if(fgets(command,COMMAND_SIZE - 2, stdin) == NULL) {
            printf("Fgets error\n");
            exit(EXIT_FAILURE);
        }
        command[strcspn(command, "\n")] = '\0';

        if(strcmp(command, "HELP") == 0) {
            help_function();
        }else {
            execute_instruction(&cpu,command);
        }
    }
}