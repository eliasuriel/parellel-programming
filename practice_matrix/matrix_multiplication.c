/*  
Elías Uriel Velázquez Rojas 
A01639716

Este codigo se realizo con la ayuda de:
https://www.tutorialspoint.com/matrix-multiplication-algorithm#
https://github.com/VictorRodriguez/parallel-programming-lecture/blob/main/labs/07/trapezoidal_omp_1.c

*/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 100

#define ColA 5
#define RowA 4
#define ColB 4
#define RowB 5



int main(int argc, char** argv) {
    int threadcnt = 1;
    if(argc > 1){
        threadcnt = atoi(argv[1]);
    }

    omp_set_num_threads(threadcnt);
    printf("OMP defined, threadcr = %d\n", threadcnt);

    int A[RowA][ColA]={};
    int B[RowB][ColB]={};
    
    for (int i= 0; i< RowA; i++){
        for (int j= 0; j< ColA; j++){
            A[i][j] = rand() % 10;
        }
    }
    for (int i= 0; i< RowB; i++){
        for (int j= 0; j< ColB; j++){
            B[i][j] = rand() % 10;
        }
    }
    printf("Matriz 1 ");
    printf("\n");
    for(int i = 0; i < RowA; i++){
        for(int j = 0; j < ColA; j++){
            printf("%d\t",A[i][j]);
        }
      printf("\n");
    }
    printf("\n");
    printf("Matriz 2");
    printf("\n");
    for(int i = 0; i < RowB; i++){
        for(int j = 0; j < ColB; j++){
            printf("%d\t",B[i][j]);
        }
      printf("\n");
    }
    printf("\n");
    int RowN = sizeof(A)/sizeof(A[0]);
    int ColN = sizeof(A[0]) / sizeof(A[0][0]);
    int Rowm = sizeof(B)/sizeof(B[0]);
    int ColM = sizeof(B[0]) / sizeof(B[0][0]);
    int i,j,k;
    if (ColN != Rowm) {
      printf("Column of first matrix should be equal to row of second matrix");
        
    }
    else{ 
        int C[N][N]={};
        #pragma omp parallel for private(i,j,k) shared (A, B, C)
        for (i = 0; i < RowN; ++i) {
            for (j = 0; j < ColM; ++j) {
                for (k = 0; k < Rowm; ++k) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        printf("Matrix C ");
        printf("\n");
        for(int i = 0; i < RowN; i++){
            for(int j = 0; j < ColM; j++){
                printf("%d\t",C[i][j]);
            }
            printf("\n");
        }
    }
    printf("Termino todo");
    return 0;

}
