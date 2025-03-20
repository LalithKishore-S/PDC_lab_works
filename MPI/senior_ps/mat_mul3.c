#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define n1 4 // Rows of A
#define m1 3 // Columns of A (must match n2)
#define n2 3 // Rows of B (must match m1)
#define m2 2 // Columns of B
// Resulting C will be of size (n1 × m2)

void print_matrix(int rows, int cols, int mat[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure n1 is divisible by size for equal row distribution
    if (n1 % size != 0) {
        if (rank == 0) {
            printf("Number of rows in A (n1) must be divisible by number of processes.\n");
        }
        MPI_Finalize();
        return 0;
    }

    int A[n1][m1], B[n2][m2], C[n1][m2]; // Full matrices (used only by rank 0)
    int local_A[n1 / size][m1], local_C[n1 / size][m2]; // Local matrices for each process

    if (rank == 0) {
        // Initialize matrices A and B
        printf("Matrix A:\n");
        for (int i = 0; i < n1; i++) {
            for (int j = 0; j < m1; j++) {
                A[i][j] = i + j + 1; // Example values
            }
        }
        print_matrix(n1, m1, A);

        printf("\nMatrix B:\n");
        for (int i = 0; i < n2; i++) {
            for (int j = 0; j < m2; j++) {
                B[i][j] = (i + j) % 3 + 1; // Example values
            }
        }
        print_matrix(n2, m2, B);
        printf("\n");
    }

    // Scatter rows of A to all processes
    MPI_Scatter(A, (n1 / size) * m1, MPI_INT, local_A, (n1 / size) * m1, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast entire matrix B to all processes
    MPI_Bcast(B, n2 * m2, MPI_INT, 0, MPI_COMM_WORLD);

    // Compute local matrix multiplication (local_C = local_A * B)
    for (int i = 0; i < (n1 / size); i++) { // Each process handles (n1 / size) rows
        for (int j = 0; j < m2; j++) {
            local_C[i][j] = 0;
            for (int k = 0; k < m1; k++) {
                local_C[i][j] += local_A[i][k] * B[k][j];
            }
        }
    }

    // Gather all computed rows of C from processes
    MPI_Gather(local_C, (n1 / size) * m2, MPI_INT, C, (n1 / size) * m2, MPI_INT, 0, MPI_COMM_WORLD);

    // Root process prints final result
    if (rank == 0) {
        printf("Resultant Matrix C (A * B):\n");
        print_matrix(n1, m2, C);
    }

    MPI_Finalize();
    return 0;
}
