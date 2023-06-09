#include <stdio.h>

#define SIZE 32

volatile int A[SIZE][SIZE];

void baseline(void)
{
    int i, j;

    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            A[i][j] = i + j;
}

int permutation(void) {

    int i, j;

    for (j = 0; j < SIZE; j++)
        for (i = 0; i < SIZE; i++)
            A[i][j] = i + j;

    return 0;
}

int unrolling(void) {

    int i, j;
    
    for (i = 0; i <= SIZE; i+=2){
        for (j = 0; j <= SIZE; j+=2){
            A[i][j] = i + j;
            A[i+1][j+1] = i + j;
        }
    }
    return 0;
}

int tiling(void) {

    int i, j;

    for (i = 0; i <= SIZE; i+=2){
        for (j = 0; j <= SIZE; j+=2){
            A[i][j] = i + j;
            A[i+1][j+1] = i + j;
        }
    }
    return 0;
}

int main(void)
{
    baseline();
    permutation();
    unrolling();
    tiling();
    return 0;
}
