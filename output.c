#include "input.h"


void printTape(char *tape, int maxTapeSize, FILE *output) {

    int i = 0;
    while (tape[i] != '\0' && i < maxTapeSize) {
        printf("%c", tape[i]);
        fprintf(output, "%c", tape[i]);
        i++;
    }
    printf("\n");
    fprintf(output, "\n");
}

void printHead(int headState, int maxTapeSize, FILE *output) {
    printf("Tape");
    fprintf(output, "Tape");
    printf("\n");
    fprintf(output, "\n");
    int i = 0;
    while (i != maxTapeSize) {
        if (i == headState){
            printf("v");
            fprintf(output, "v");
        }
        if (i != headState){
            printf("_");
            fprintf(output, "_");
        }
        i++;
    }
    printf("\n");
    fprintf(output, "\n");
}

void printCommands(struct Command **arr, int maxArraySize, FILE *output){
    printf("Command matrix");
    fprintf(output, "Command matrix");
    printf("\n");
    fprintf(output, "\n");
    for (int k = 0; k < maxArraySize; ++k) {
        for (int j = 0; j < maxArraySize; ++j) {
            printf("%c%d%c  ", arr[k][j].newChar, arr[k][j].newStateNum, arr[k][j].move);
            fprintf(output, "%c%d%c  ", arr[k][j].newChar, arr[k][j].newStateNum, arr[k][j].move);
        }
        printf("\n");
        fprintf(output, "\n");
    }
}

void printCommand(struct Command command, char lastChar, int lastStateNum, FILE *output){
    printf("Running command");
    fprintf(output, "Running command");
    printf("\n");
    fprintf(output, "\n");
    printf("%cq%d-%cq%d%c", lastChar, lastStateNum, command.newChar, command.newStateNum, command.move);
    fprintf(output, "%cq%d-%cq%d%c", lastChar, lastStateNum, command.newChar, command.newStateNum, command.move);
    printf("\n");
    fprintf(output, "\n");
}
