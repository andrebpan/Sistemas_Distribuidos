#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void multiplicar_mat(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) { 
            C[i][j] = 0;
            for (int k = 0; k < n; k++) { 
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

//alocar memória dinamicamente
int** allocateMatrix(int n) {
    int **matrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int *)malloc(n * sizeof(int));
    }
    return matrix;
}

void freeMatrix(int **matrix, int n) {
    for (int i = 0; i < n; i++) { 
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int n = 2000; //tamanho da matriz
    int **A = allocateMatrix(n);
    int **B = allocateMatrix(n);
    int **C = allocateMatrix(n);

    //inicializando matrizes A e B
    for (int i = 0; i < n; i++) {  
        for (int j = 0; j < n; j++) {  
            A[i][j] = i + j;
            B[i][j] = i + j;
        }
    }

    //tempo de execução
    clock_t start = clock();
    multiplicar_mat(A, B, C, n);
    clock_t end = clock();

    double tempo_exec = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTempo de execução serial: %.4f segundos\n", tempo_exec);

    freeMatrix(A, n);
    freeMatrix(B, n);
    freeMatrix(C, n);

    return 0;
}
