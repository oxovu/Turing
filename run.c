#include "input.h"


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
