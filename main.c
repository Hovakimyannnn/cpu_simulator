#include "cpu.h"

int main() {
    CPU cpu;
    char command[COMMAND_SIZE];

    printf("CPU Simulator\n");

    while (1) {
        if (fgets(command, COMMAND_SIZE - 2, stdin) == NULL) {
            printf("Input error\n");
            exit(EXIT_FAILURE);
        }
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "HELP") == 0) {
            help_function();
        } else {
            execute_instruction(&cpu, command);
        }
    }

    return 0;
}
