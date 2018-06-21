#include "input.h"

int step(int headState, int lastStateNum, char *tape, char *symbols, int *states, struct Command **arr, int maxSize,
         int quit, FILE *output) {
    while (quit != 1000) {
        quit++;
        char lastChar = tape[headState];
        int i = arrayContainsChar(lastChar, symbols, maxSize);
        int j = arrayContainsInt(lastStateNum, states, maxSize);
        printCommand(arr[i][j], lastChar, lastStateNum, output);
        lastStateNum = arr[i][j].newStateNum;
        tape[headState] = arr[i][j].newChar;
        switch (arr[i][j].move) {
            case 'L':
                headState--;
                break;
            case 'R':
                headState++;
                break;
        }
        if (headState < 0) {
            printf("Head index out of bounds");
            exit(200);
        }
        if (arr[i][j].move == 'S') {
            break;
        }
    }

    if (quit == 1000) {
        printf("Runtime error");
        exit(201);
    }
    return headState;
}

