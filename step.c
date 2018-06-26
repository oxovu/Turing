#include "input.h"

int
step(int headState, int lastStateNum, char *tape, char *symbols, int *states, struct Command **arr, int maxArraySize,
     int maxTapeSize, int quit, FILE *output, _Bool flag) {
    while (quit != 1000) {
        quit++;
        char lastChar = tape[headState];
        int i = arrayContainsChar(lastChar, symbols, maxArraySize); // поиск индексов
        int j = arrayContainsInt(lastStateNum, states, maxArraySize);
        if (i == -1 || j == -1) {
            printf("error 203: Don't have command for the state\n");
            exit(203);
        }
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
        if (headState == maxTapeSize) {
            maxTapeSize++;
            tape = realloc(tape, maxTapeSize * sizeof(char));
            if (tape == NULL) {
                printf("error 102: Memory allocation error\n");
                exit(102);
            }
            tape[maxTapeSize - 1] = ' ';
        }
        if (headState < 0) {
            printf("error 200: Head index out of bounds\n");
            exit(200);
        }
        if (arr[i][j].move == 'S') {
            break;
        }
        printHead(headState, maxTapeSize, output);
        printTape(tape, maxTapeSize, output);
        if (flag) {
            char comand[2]; //сюда будет записана команда
            while (1) {
                scanf(" %2s", comand);
                fflush(stdin); //очистка буфера
                if (!strcmp(comand, "b")) {
                    printf("Program break\n");
                    fclose(output);
                    exit(0);
                }
                if (!strcmp(comand, "s")) {
                    break;
                }
                if (!strcmp(comand, "f")) {
                    flag = false;
                    break;
                } else printf("No such instruction. Enter other\n");
            }
        }
    }

    printHead(headState, maxTapeSize, output);
    printTape(tape, maxTapeSize, output);

    if (quit == 1000) {
        printf("error 201: Runtime error\n");
        exit(201);
    }
    return headState;
}

