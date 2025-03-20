#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NUM_TESTS 2  // Number of iterations for averaging
#define MAX_SIZE 10  // Maximum message size (1 MB)
#define MIN_SIZE 1  // Minimum message size (1 byte)

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) {
            printf("This program requires at least two MPI processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    double start, end, total_time, avg_time, bandwidth;
    char *message = (char *)malloc(MAX_SIZE * sizeof(char));

    if (rank == 0) {
        printf("Message Size (Bytes)  |  Avg Comm Time (s)  |  Bandwidth (MB/s)\n");
        printf("-------------------------------------------------------------\n");
    }

    for (int msg_size = MIN_SIZE; msg_size <= MAX_SIZE; msg_size *= 2) {  // Test different message sizes
        total_time = 0.0;

        for (int i = 0; i < NUM_TESTS; i++) {
            MPI_Barrier(MPI_COMM_WORLD);  // Synchronize before measuring
            start = MPI_Wtime();  // Start timing

            MPI_Send(message, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(message, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            end = MPI_Wtime();  // End timing
            total_time += (end - start);
        }

        avg_time = total_time / NUM_TESTS;
        bandwidth = (msg_size / (1024.0 * 1024.0)) / avg_time;  // Convert bytes to MB

        if (rank == 0) {
            printf("%10d          |  %10.6f       |  %10.2f MB/s\n", msg_size, avg_time, bandwidth);
        }
    }

    free(message);
    MPI_Finalize();
    return 0;
}
