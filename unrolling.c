#include<stdio.h>



int main(void) {

    int SIZE = 16;
    int i, j;
    int A[SIZE][SIZE];
    for (i = 0; i <= SIZE; i+=2){
        for (j = 0; j <= SIZE; j+=2){
            A[i][j] = i + j;
            A[i+1][j+1] = i + j;
        }
    }
    return 0;
}
