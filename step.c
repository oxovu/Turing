#include "input.h"

//_Bool commandCompare(struct Command command1, struct Command command2) {
//    if (command1.newStateNum == command2.newStateNum && command1.newChar == command2.newChar &&
//            command1.move == command2.move) {
//        return true;
//    }
//    return false;
//}

int
step(int headState, int lastStateNum, char *tape, char *symbols, int *states, struct Command **arr, int maxArraySize,
     int maxTapeSize, int quit, FILE *output, _Bool flag) {
    while (quit != 1000) {
        quit++;
        char lastChar = tape[headState];
        int i = arrayContainsChar(lastChar, symbols, maxArraySize);
        int j = arrayContainsInt(lastStateNum, states, maxArraySize);
        if (arr[i][j].newStateNum == NULL || arr[i][j].newChar == NULL) {
            printf("Don't have command for the state");
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
                printf("Memory allocation error");
                exit(102);
            }
            tape[maxTapeSize - 1] = ' ';
        }
        if (headState < 0) {
            printf("Head index out of bounds");
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
                    printf("Program break");
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

    if (quit == 1000) {
        printf("Runtime error");
        exit(201);
    }
    return headState;
}

