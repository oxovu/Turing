#include "input.h"

#define ARRAY_SIZE 7 //начальная длина матрицы смежности
#define TAPE_SIZE 4 //начальная длина ленты

int main(int argc, char *argv[]) { //добавить в ленте проверку на пустой символ B
    FILE *inputOne;
    FILE *inputTwo;
    FILE *output;

    if (argc == 1) {
        //описание программы
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

    char lastChar; //переменные для чтения из файла состояний
    char lastState;
    int lastStateNum;
    char dash;
    char newChar;
    char newState;
    int newStateNum;
    char move;
    int maxArraySize = ARRAY_SIZE; //размер матрицы смежности

    int *states; //вспомогательный массив, хранящий состояния
    states = calloc(ARRAY_SIZE, sizeof(states));
    char *symbols; //вспомагательный массив, хранящий символы ленты
    symbols = calloc(ARRAY_SIZE, sizeof(symbols));

    int i = 0; //счетчик для заполнения марицы смежности
    int statesCount = 0; //текущий индекс массива states
    int symbolsCount = 0; //текущий индекс массива symbols
    struct Command blankCommand = {'\0', 0, '\0'}; //пустая команда

    struct Command **command; //таблица смежности
    command = calloc(ARRAY_SIZE, sizeof(struct Command *));

    for (int j = 0; j < ARRAY_SIZE; ++j) {
        command[j] = calloc(ARRAY_SIZE, sizeof(struct Command));
    }

    if (command == NULL) {
        printf("Memory allocation error");
        exit(102);
    }

    int ret = 0; //сечтчик fscanf

    while (true) { //чтение файла с командами
        ret = fscanf(inputOne, " %c %c %d %c %c %c %d %c", &lastChar, &lastState, &lastStateNum, &dash, &newChar,
                     &newState, &newStateNum, &move);
        if (ret == EOF) break;

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

        i++;
        if (i == maxArraySize) { //увеличение матрицы при необходимости
            maxArraySize = maxArraySize * 2;
            states = realloc(states, maxArraySize * sizeof(int));
            symbols = realloc(symbols, maxArraySize * sizeof(char));
            command = realloc(command, maxArraySize * sizeof(struct Command *));

            for (int j = 0; j < maxArraySize / 2; ++j) {
                command[j] = realloc(command[j], maxArraySize * sizeof(struct Command));
                for (int k = maxArraySize / 2; k < maxArraySize; ++k) {
                    command[j][k] = blankCommand;
                }
            }

            for (int k = maxArraySize / 2; k < maxArraySize; ++k) {
                command[k] = calloc(maxArraySize, sizeof(struct Command));
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

    printCommands(command, maxArraySize, output);

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
        maxTapeSize = maxTapeSize + 1;
        head = realloc(head, maxTapeSize * sizeof(int));
        tape = realloc(tape, maxTapeSize * sizeof(char));
        for (int j = maxTapeSize - 1; j < maxTapeSize; ++j) {
            tape[j] = '\0';
        }
    }
    fseek(inputTwo, 0, SEEK_SET);
    ret = fscanf(inputTwo, " %s", head);

    for (int l = 0; l < maxTapeSize; ++l) {
        if (head[l] != '_' && head[l] != 'v' && head[l] != '\0') {
            printf("Wrong input char %c in %s line 1", head[l], argv[2]);
            exit(108);
        }
        if (head[l] == 'v') {
            headState = l;
        }
    }

    if (headState == -1) {
        printf("Wrong input data in %s line 1", argv[2]);
        exit(109);
    }

    ret = fscanf(inputTwo, " %s", tape);
    if (ret == NULL) {
        printf("No input data in %s line 2", argv[2]);
        exit(107);
    }

    fclose(inputTwo);

    printHead(headState, maxTapeSize, output);
    printTape(tape, maxTapeSize, output);

    if (!strcmp(argv[4], "-a")) {

        int quit = 0; //количество шагов

        headState = step(headState, 1, tape, symbols, states, command, maxArraySize, quit, output);

        printHead(headState, maxTapeSize, output);
        printTape(tape, maxTapeSize, output);
    }

    exit(0);
}

int arrayContainsChar(char symbol, char *arr, int maxSize) { //возвращает индекс символа или -1 если символ не найден
    for (int i = 0; i < maxSize; ++i) {
        if (arr[i] == symbol) return i;
    }
    return -1;
}

int arrayContainsInt(int symbol, int *arr, int maxSize) { //возвращает индекс числа или -1 если число не найдено
    for (int i = 0; i < maxSize; ++i) {
        if (arr[i] == symbol) return i;
    }
    return -1;
}

