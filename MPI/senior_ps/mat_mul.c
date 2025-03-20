#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 4 // Matrix size (NxN)

// Function to print a matrix
void print_matrix(int *mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", mat[i * cols + j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure size is compatible with N (assuming square matrices)
    if (N % size != 0) {
        if (rank == 0) {
            printf("Matrix size N must be divisible by number of processes.\n");
        }
        MPI_Finalize();
        return 0;
    }

    int A[N * N], B[N * N], C[N * N]; // Full matrices (only rank 0 uses full A, C)
    int local_A[N * (N / size)], local_C[N * (N / size)]; // Local matrices for each process

    if (rank == 0) {
        // Initialize matrices A and B
        printf("Matrix A:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i * N + j] = i + j + 1; // Example values
            }
        }
        print_matrix(A, N, N);

        printf("\nMatrix B:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                B[i * N + j] = (i == j) ? 1 : 2; // Example values (identity-like)
            }
        }
        print_matrix(B, N, N);
        printf("\n");
    }

    // Scatter rows of A to all processes
    MPI_Scatter(A, N * (N / size), MPI_INT, local_A, N * (N / size), MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast entire matrix B to all processes
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Compute local matrix multiplication (local_C = local_A * B)
    for (int i = 0; i < (N / size); i++) { // Each process handles (N / size) rows
        for (int j = 0; j < N; j++) {
            local_C[i * N + j] = 0;
            for (int k = 0; k < N; k++) {
                local_C[i * N + j] += local_A[i * N + k] * B[k * N+j];
            }
        }
    }

    // Gather all computed rows of C from processes
    MPI_Gather(local_C, N * (N / size), MPI_INT, C, N * (N / size), MPI_INT, 0, MPI_COMM_WORLD);

    // Root process prints final result
    if (rank == 0) {
        printf("Resultant Matrix C (A * B):\n");
        print_matrix(C, N, N);
    }

    MPI_Finalize();
    return 0;
}

