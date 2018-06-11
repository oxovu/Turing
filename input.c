#include "input.h"

#define ARRAY_SIZE 5
#define TAPE_SIZE 12

int main(int argc, char *argv[]) {
    FILE *inputOne;
    FILE *inputTwo;
    FILE *output;

    if (argc == 1) {
        //вызвать описание программы
        exit(0);
    }

    if (argc == 5 && (strcmp(argv[4], "-a") == 0 || strcmp(argv[4], "-o") == 0)) {

        inputOne = fopen(argv[1], "rt");

        if (inputOne == NULL) {
            printf("Can not find file %s", argv[1]);
            exit(100);
        }

        inputTwo = fopen(argv[2], "rt");
        if (inputTwo == NULL) {
            printf("Can not find file %s", argv[2]);
            exit(100);
        }

        output = fopen(argv[3], "wr");
        if (output == NULL) {
            printf("Can not find file %s", argv[3]);
        }
    }

    if (argc != 5) {
        printf("Wrong number of arguments");
        exit(101);
    }

    char lastChar;
    char lastState;
    int lastStateNum;
    char dash;
    char newChar;
    char newState;
    int newStateNum;
    char move;
    int maxArraySize = ARRAY_SIZE;

    int *states;
    states = calloc(ARRAY_SIZE, sizeof(states));
    char *symbols;
    symbols = calloc(ARRAY_SIZE, sizeof(symbols));

    int i = 0;
    int statesCount = 0;
    int symbolsCount = 0;


    int ret = 0;

    struct Command **command;
    command = calloc(ARRAY_SIZE, sizeof(struct Command *));

    for (int j = 0; j < ARRAY_SIZE; ++j) {
        command[j] = calloc(ARRAY_SIZE, sizeof(struct Command));
    }

    if (command == NULL) {
        printf("Memory allocation error");
        exit(102);
    }

    while (true) {
        ret = fscanf(inputOne, " %c %c %d %c %c %c %d %c", &lastChar, &lastState, &lastStateNum, &dash, &newChar,
                     &newState, &newStateNum, &move);
        if (ret == EOF) break;

        //поменять коды ошибок и сообщения

        if (ret != 8) {
            printf("Wrong number of input chars");
            exit(103);
        }

        if (lastState != 'q' || newState != 'q') {
            printf("Can not find states");
            exit(104);
        }

        if (dash != '-') {
            printf("There is no new part");
            exit(105);
        }


        if (move != 'L' && move != 'R' && move != 'H' && move != 'S') {
            printf("Wrong move command");
            // printf("Wrong input data in %s line %d", argv[1], i + 1);
            exit(106);
        }

        if (arrayContainsChar(lastChar, symbols, maxArraySize) == -1) {
            symbols[symbolsCount] = lastChar;
            symbolsCount++;
        }

        if (arrayContainsInt(lastStateNum, states, maxArraySize) == -1) {
            states[statesCount] = lastStateNum;
            statesCount++;
        }

        command[arrayContainsChar(lastChar, symbols, maxArraySize)][arrayContainsInt(lastStateNum, states,
                                                                                maxArraySize)].newChar = newChar;
        command[arrayContainsChar(lastChar, symbols, maxArraySize)][arrayContainsInt(lastStateNum, states,
                                                                                maxArraySize)].newStateNum = newStateNum;
        command[arrayContainsChar(lastChar, symbols, maxArraySize)][arrayContainsInt(lastStateNum, states,
                                                                                maxArraySize)].move = move;

        i++;
        if (i == maxArraySize) {
            maxArraySize = ARRAY_SIZE * (i - ARRAY_SIZE + 1);
            states = realloc(states, maxArraySize * sizeof(int));
            symbols = realloc(symbols, maxArraySize * sizeof(char));
            command = realloc(command, maxArraySize * sizeof(struct Command *));

            for (int j = 0; j < i - 1; ++j) {
                command[i] = realloc(command, maxArraySize * sizeof(struct Command));
            }
        }
    }

    fclose(inputOne);

    //убрать печать
    for (int k = 0; k < maxArraySize; ++k) {
        for (int j = 0; j < maxArraySize; ++j) {
            printf("%c%d%c  ", command[k][j].newChar, command[k][j].newStateNum, command[k][j].move);
        }
        printf("\n");
    }

    char *head;
    head = calloc(TAPE_SIZE, sizeof(char));
    int headState = -1;
    char *tape;
    tape = calloc(TAPE_SIZE, sizeof(char));
    int maxTapeSize = TAPE_SIZE;

    ret = fscanf(inputTwo, " %s", head);

    if (ret == NULL) {
        printf("No input data in %s line 1", argv[2]);
        exit(107);
    }

    while (head[maxTapeSize] != '\0'){
        maxTapeSize = maxTapeSize * 2;
        head = realloc(head, maxTapeSize * sizeof(int));
    }

    for (int l = 0; l < maxTapeSize; ++l) {
        if (head[l] != '_' && head[l] != 'v' && head[l] != '\0'){
            printf("Wrong input char %c in %s line 1", head[l], argv[2]);
            exit(108);
        }
        if (head[l] == 'v') {
            headState = l;
        }
    }

    if (headState == -1){
        printf("Wrong input data in %s line 1", argv[2]);
        exit(109);
    }


    ret = fscanf(inputTwo, " %s", tape);
    if (ret == NULL) {
        printf("No input data in %s line 2", argv[2]);
        exit(107);
    }

    fclose(inputTwo);

    //убрать печать
    printf("%d", headState);
    printf("\n");
    for (int m = 0; m < maxTapeSize; ++m) {
        printf("%c", tape[m]);
    }
    exit(0);
}

int arrayContainsChar(char symbol, char *arr, int maxSize) {
    for (int i = 0; i < maxSize - 1; ++i) {
        if (arr[i] == symbol) return i;
    }
    return -1;
}

int arrayContainsInt(int symbol, int *arr, int maxSize) {
    for (int i = 0; i < maxSize - 1; ++i) {
        if (arr[i] == symbol) return i;
    }
    return -1;
}