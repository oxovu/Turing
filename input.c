#include "input.h"

// в этом примере используется
//myFile myFile2 outFile -a

#define ARRAY_SIZE 7 //начальная длина матрицы смежности
#define TAPE_SIZE 4 //начальная длина ленты

void inf() {
    printf("Turing simulator\nto run the program type in command line\nTuring.exe input1.txt input2.txt output.txt -a/-o"
                   " [-p]\n-a runs the program,\n-o runs debug,\n-p gives you information about your input data."
                   "\ninput1.txt should contain such commands:\naqn-bqkD, where a and b are the symbols on the tape (the"
                   " first state is always q1),\nqn"
                   " and qk are the states of machine,\nD is the direction of move (L - left, R - right, H - no move,"
                   " S - stop).\ninput2.txt should contain the initial tape in such structure:\n_____v____\n1210392910\nif"
                   " you run debug\ns - one step,\nb - break,\nf - follow the program in standart mode.\nExample:"
                   "\ninput1.txt\n0q1-1q1L\n1q1-1q1L\n2q1-1q1S\ninput2.txt\n__v__\n20101\noutput.txt\nTape\n__v__\n20101"
                   "\nRunning command\n1q1-1q1L\nTape\n_v___\n20101\nRunning command\n0q1-1q1L\nTape\nv____\n21101"
                   "\nRunning command\n2q1-1q1S\nTape\nv____\n11101");
}

int main(int argc, char *argv[]) {
    FILE *inputOne;
    FILE *inputTwo;
    FILE *output;

    _Bool flag = false;

    if (argc == 1) {
        inf(); // информация о программе
        exit(0);
    }

    if ((argc == 5) && (strcmp(argv[4], "-a") == 0 || strcmp(argv[4], "-o") == 0) ||
        ((argc == 6) && (strcmp(argv[4], "-a") == 0 || strcmp(argv[4], "-o") == 0) && (strcmp(argv[5], "-p") == 0))) {

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
        if (argc != 6 || (argc == 6 && (strcmp(argv[5], "-p") != 0))) {
            printf("Wrong number of arguments");
            exit(101);
        }
    }

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
        maxTapeSize = maxTapeSize + 1;
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

    ret = fscanf(inputTwo, " %s", tape);
    if (ret == NULL) {
        printf("No input data in %s line 2", argv[2]);
        exit(107);
    }

    fclose(inputTwo);

    printHead(headState, maxTapeSize, output);
    printTape(tape, maxTapeSize, output);

    if (strcmp(argv[4], "-o") == 0) {
        flag = true;
    }

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


