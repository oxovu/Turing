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

int arrayContainsChar(char symbol, char *arr, int maxSize);
int arrayContainsInt(int symbol, int *arr, int maxSize);

#endif //TURING_INPUT_H
