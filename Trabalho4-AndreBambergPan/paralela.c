#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void multiplicar_mat(int *A, int *B, int *C, int start_row, int end_row, int n) {
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < n; j++) {
            C[i * n + j] = 0;
            for (int k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}

int* allocateMatrix(int n) {
    return (int *)malloc(n * n * sizeof(int));
}

void freeMatrix(int *matrix) {
    free(matrix);
}

int main(int argc, char *argv[]) {
    int n = 2000;  //tamanho da matriz
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *A, *B, *C;
    int *local_A, *local_C;
    int rows_per_process;

    if (rank == 0) {
        A = allocateMatrix(n);
        B = allocateMatrix(n);
        C = allocateMatrix(n);

        //inicializando matrizes A e B
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i * n + j] = i + j;
                B[i * n + j] = i + j;
            }
        }
    }

    //broadcast da matriz B para todos os processos
    B = allocateMatrix(n);
    MPI_Bcast(B, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    //dividindo as linhas da matriz A entre os processos
    rows_per_process = n / size;
    local_A = (int *)malloc(rows_per_process * n * sizeof(int));
    local_C = (int *)malloc(rows_per_process * n * sizeof(int));

    //scatter das linhas da matriz A(comunicação entre os processos, evita sobreposição)
    MPI_Scatter(A, rows_per_process * n, MPI_INT, local_A, rows_per_process * n, MPI_INT, 0, MPI_COMM_WORLD);

    //tempo de execução
    double start = MPI_Wtime();
    multiplicar_mat(local_A, B, local_C, 0, rows_per_process, n);
    double end = MPI_Wtime();

    //coletando os resultados de C
    MPI_Gather(local_C, rows_per_process * n, MPI_INT, C, rows_per_process * n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nTempo de execução paralelo: %.4f segundos\n", end - start);

        freeMatrix(A);
        freeMatrix(C);
    }

    free(local_A);
    free(local_C);
    freeMatrix(B);

    MPI_Finalize();
    return 0;
}
