#include<stdio.h>

const int SIZE = 16

int main(void) {
    for (i = 0; i < SIZE; i+=2)
        for (j = 0; j < SIZE; j+=2){
            A[i][j] = i + j;
            A[i+1][j+1] = i + j;
        }

    return 0;
}