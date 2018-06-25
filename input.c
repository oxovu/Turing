#include "input.h"

// в этом примере используется
//myFile myFile2 outFile -a

#define ARRAY_SIZE 7 //начальная длина матрицы смежности
#define TAPE_SIZE 4 //начальная длина ленты


int input(FILE *inputOne, FILE *inputTwo, FILE *output, int argc, char *argv[]) {

    _Bool flag = false;

    //переменные для чтения из файла состояний

    char lastChar; // символ на ленте
    char lastState;
    int lastStateNum; // состояние в котором находится лента
    char dash;
    char newChar; // символ, который нужно записать
    char newState;
    int newStateNum; // со //переменные для чтения из файла состоянийстояние, в которе должна перейти лента
    char move; //
    int maxArraySize = ARRAY_SIZE; //размер матрицы смежности

    int *states; //вспомогательный массив, хранящий состояния
    states = calloc(ARRAY_SIZE, sizeof(states));
    if (states == NULL) {
        printf("Memory allocation error");
        exit(102);
    }
    char *symbols; //вспомагательный массив, хранящий символы ленты
    symbols = calloc(ARRAY_SIZE, sizeof(symbols));
    if (symbols == NULL) {
        printf("Memory allocation error");
        exit(102);
    }

    int i = 0; //счетчик для заполнения марицы смежности
    int statesCount = 0; //текущий индекс массива states
    int symbolsCount = 0; //текущий индекс массива symbols
    struct Command blankCommand = {'\0', 0, '\0'}; //пустая команда

    struct Command **command; //таблица смежности
    command = calloc(ARRAY_SIZE, sizeof(struct Command *));

    if (command == NULL) {
        printf("Memory allocation error");
        exit(102);
    }

    for (int j = 0; j < ARRAY_SIZE; ++j) {
        command[j] = calloc(ARRAY_SIZE, sizeof(struct Command));
        if (command[j] == NULL) {
            printf("Memory allocation error");
            exit(102);
        }
    }


    int ret = 0; //сечтчик fscanf

    while (true) { //чтение файла с командами

        ret = fscanf(inputOne, "%c%c%c%d%c%c%c%d%c\n", &dash, &lastChar, &lastState, &lastStateNum, &dash, &newChar,
                     &newState, &newStateNum, &move);
        if (ret == EOF) break;

        if (ret != 9) {
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

        i++;
        if (i == maxArraySize) { //увеличение матрицы при необходимости
            maxArraySize = maxArraySize * 2;
            states = realloc(states, maxArraySize * sizeof(int));
            if (states == NULL) {
                printf("Memory allocation error");
                exit(102);
            }
            symbols = realloc(symbols, maxArraySize * sizeof(char));
            if (symbols == NULL) {
                printf("Memory allocation error");
                exit(102);
            }
            command = realloc(command, maxArraySize * sizeof(struct Command *));
            if (command == NULL) {
                printf("Memory allocation error");
                exit(102);
            }

            for (int j = 0; j < maxArraySize / 2; ++j) {
                command[j] = realloc(command[j], maxArraySize * sizeof(struct Command));
                if (command[j] == NULL) {
                    printf("Memory allocation error");
                    exit(102);
                }
                for (int k = maxArraySize / 2; k < maxArraySize; ++k) {
                    command[j][k] = blankCommand;
                }
            }

            for (int k = maxArraySize / 2; k < maxArraySize; ++k) {
                command[k] = calloc(maxArraySize, sizeof(struct Command));
                if (command[k] == NULL) {
                    printf("Memory allocation error");
                    exit(102);
                }
            }
        }

        //запись команд в матрицу смежности

        command[arrayContainsChar(lastChar, symbols, maxArraySize)][arrayContainsInt(lastStateNum, states,
                                                                                     maxArraySize)].newChar = newChar;
        command[arrayContainsChar(lastChar, symbols, maxArraySize)][arrayContainsInt(lastStateNum, states,
                                                                                     maxArraySize)].newStateNum = newStateNum;
        command[arrayContainsChar(lastChar, symbols, maxArraySize)][arrayContainsInt(lastStateNum, states,
                                                                                     maxArraySize)].move = move;
    }

    fclose(inputOne);

    if (argc == 6 && (strcmp(argv[5], "-p") == 0)) {
        printCommands(command, maxArraySize, output);
    }

    char *head; //переменные для чтения файла с лентой
    head = calloc(TAPE_SIZE, sizeof(char));
    if (head == NULL) {
        printf("Memory allocation error");
        exit(102);
    }
    int headState = -1; //индекс положения головки машины
    char *tape;
    tape = calloc(TAPE_SIZE, sizeof(char));
    if (tape == NULL) {
        printf("Memory allocation error");
        exit(102);
    }
    int maxTapeSize = TAPE_SIZE;

    ret = fscanf(inputTwo, " %s", head);

    if (ret == NULL) {
        printf("No input data in %s line 1", argv[2]);
        exit(107);
    }

    while (head[maxTapeSize] != '\0') { //увеличение размера ленты при необходимости
        maxTapeSize++;
        head = realloc(head, maxTapeSize * sizeof(int));
        if (head == NULL) {
            printf("Memory allocation error");
            exit(102);
        }
        tape = realloc(tape, maxTapeSize * sizeof(char));
        if (tape == NULL) {
            printf("Memory allocation error");
            exit(102);
        }
        for (int j = maxTapeSize - 1; j < maxTapeSize; ++j) {
            tape[j] = '\0';
        }
    }
    fseek(inputTwo, 0, SEEK_SET);
    ret = fscanf(inputTwo, " %s", head);

    if (ret == NULL) {
        printf("No input data in %s line 1", argv[2]);
        exit(107);
    }

    for (int l = 0; l < maxTapeSize; ++l) {
        if (head[l] != '_' && head[l] != 'v' && head[l] != '\0') {
            printf("Wrong input char %c in %s line 1", head[l], argv[2]);
            exit(108);
        }
        if (head[l] == 'v') {
            headState = l;
            break;
        }
    }

    if (headState == -1) {
        printf("Can not find the head");
        exit(202);
    }


    fscanf(inputTwo, "\n");

    for (int n = 0; n < maxTapeSize; ++n) {
        tape[n] = ' ';
    }

    for (int n = 0; n < maxTapeSize; ++n) {
        fscanf(inputTwo, "%1c", &tape[n]);
    }

    fclose(inputTwo);

    if (strcmp(argv[4], "-o") == 0) {
        flag = true;
    }

    printHead(headState, maxTapeSize, output);
    printTape(tape, maxTapeSize, output);

    int quit = 0; //количество шагов

    headState = step(headState, 1, tape, symbols, states, command, maxArraySize, maxTapeSize, quit, output, flag);

    printHead(headState, maxTapeSize, output);
    printTape(tape, maxTapeSize, output);

    free(head);
    free(tape);
    free(states);
    free(symbols);
    for (int m = 0; m < maxArraySize; ++m) {
        free(command[m]);
    }
    fclose(output);

    exit(0);
}


