#ifndef TURING_INPUT_H
#define TURING_INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Command {
    char newChar;
    int newStateNum;
    char move;
};

int input(FILE *inputOne, FILE *inputTwo, FILE *output, int argc, char *argv[]);

int arrayContainsChar(char symbol, char *arr, int maxSize);

int arrayContainsInt(int symbol, int *arr, int maxSize);

int step(int headState, int lastStateNum, char *tape, char *symbols, int *states, struct Command **arr, int maxArraySize, int maxTapeSize,
          int quit, FILE *output, _Bool flag);

void printTape(char *tape, int maxTapeSize, FILE *output);

void printHead(int headState, int maxTapeSize, FILE *output);

void printCommands(struct Command **arr, int maxSize, FILE *output);

void printCommand(struct Command command, char lastChar, int lastStateNum, FILE *output);

#endif //TURING_INPUT_H
