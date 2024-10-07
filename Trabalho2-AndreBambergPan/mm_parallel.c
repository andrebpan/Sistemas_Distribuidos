#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void multiplicar_mat(int **A, int **B, int **C, int n) {
    int i, j, k;
    #pragma omp parallel shared(A, B, C, n) private(i, j, k)
    {
        #pragma omp for
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) { 
                C[i][j] = 0;
                for (k = 0; k < n; k++) {  
                    C[i][j] += A[i][k] * B[k][j];
                }
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
    int n = 2100; //tamanho da matriz
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

    //execução paralela com diferentes números de threads
    int num_cores[] = {2, 4, 6, 8, 12};
    for (int t = 0; t < 5; t++) {
        omp_set_num_threads(num_cores[t]);

        //tempo de execução
        double start_time = omp_get_wtime();
        multiplicar_mat(A, B, C, n);
        double end_time = omp_get_wtime();
        double tempo_exec = end_time - start_time;

        printf("Threads: %d, Tempo paralelo: %.4f segundos\n", num_cores[t], tempo_exec);
    }

    freeMatrix(A, n);
    freeMatrix(B, n);
    freeMatrix(C, n);

    return 0;
}
