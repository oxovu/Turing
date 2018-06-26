#include "input.h"

void inf() {
    printf("Turing simulator\nto run the program type in command line\nTuring.exe input1.txt input2.txt output.txt -a/-o"
                   " [-p]\n-a runs the program,\n-o runs debug,\n-p gives you information about your input data."
                   "\ninput1.txt should contain such commands:\n-aqn-bqkD, where a and b are the symbols on the tape (the"
                   " first state is always q1),\nqn"
                   " and qk are the states of machine,\nD is the direction of move (L - left, R - right, H - no move,"
                   " S - stop).\ninput2.txt should contain the initial tape in such structure:\n_____v____\n1210392910\nif"
                   " you run debug\ns - one step,\nb - break,\nf - follow the program in standart mode.\nExample:"
                   "\ninput1.txt\n-0q1-1q1L\n-1q1-1q1L\n-2q1-1q1S\ninput2.txt\n__v__\n20101\noutput.txt\nTape\n__v__\n20101"
                   "\nRunning command\n1q1-1q1L\nTape\n_v___\n20101\nRunning command\n0q1-1q1L\nTape\nv____\n21101"
                   "\nRunning command\n2q1-1q1S\nTape\nv____\n11101\n");
}


int main(int argc, char *argv[]) {
    FILE *inputOne;
    FILE *inputTwo;
    FILE *output;

    if (argc == 1) {
        inf(); // информация о программе
        exit(0);
    }

    if ((argc == 5) && (strcmp(argv[4], "-a") == 0 || strcmp(argv[4], "-o") == 0) ||
        ((argc == 6) && (strcmp(argv[4], "-a") == 0 || strcmp(argv[4], "-o") == 0) && (strcmp(argv[5], "-p") == 0))) {

        inputOne = fopen(argv[1], "rt");

        if (inputOne == NULL) {
            printf("Can not find file %s\n", argv[1]);
            exit(100);
        }

        inputTwo = fopen(argv[2], "rt");
        if (inputTwo == NULL) {
            printf("Can not find file %s\n", argv[2]);
            exit(100);
        }

        output = fopen(argv[3], "wr");
        if (output == NULL) {
            printf("Can not find file %s", argv[3]);
        }
    }

    if (argc != 5) {
        if (argc != 6 || (argc == 6 && (strcmp(argv[5], "-p") != 0))) {
            printf("Wrong number of arguments\n");
            exit(101);
        }
    }

    input(inputOne, inputTwo, output, argc, argv);

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
