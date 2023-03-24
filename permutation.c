#include<stdio.h>

int main(void) {

    int SIZE = 16;
    int i, j;
    int A[SIZE][SIZE];

    for (j = 0; j < SIZE; j++)
        for (i = 0; i < SIZE; i++)
            A[i][j] = i + j;

    return 0;
}
