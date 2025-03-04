#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define N 3  // Matrix size

void print_matrix(int mat[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int n_procs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int A[N][N], B[N][N], C[N][N];  // Matrices
    int local_A[N];  // Store a row of A for each process
    int local_C[N];  // Store the computed row of C

    srand(time(NULL));
    if (rank == 0) {
        // Initialize matrices A and B
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = rand() % 2;  // Random 0 or 1
                B[i][j] = rand() % 2;
            }
        }
        printf("Matrix A:\n");
        print_matrix(A);
        printf("Matrix B:\n");
        print_matrix(B);
    }

    // Scatter rows of A to all processes
    MPI_Scatter(A, N, MPI_INT, local_A, N, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Broadcast matrix B to all processes
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Compute one row of C in each process
    for (int j = 0; j < N; j++) {
        local_C[j] = 0;
        for (int k = 0; k < N; k++) {
            local_C[j] += local_A[k] * B[k][j];  // Dot product
        }
    }

    // Gather the computed rows back to process 0
    MPI_Gather(local_C, N, MPI_INT, C, N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Resultant Matrix C (A x B):\n");
        print_matrix(C);
    }

    MPI_Finalize();
    return 0;
}
