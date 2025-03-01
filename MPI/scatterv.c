#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int total_elements = 6;
    int sendbuf[6] = {1, 2, 3, 4, 5, 6};  // Only root process initializes this
    int recvbuf[3]; // Max size to store received data

    // Define how many elements each process gets
    int sendcounts[3] = {1, 2, 3};  // P0 -> 1, P1 -> 2, P2 -> 3

    // Define the displacement (starting index for each process)
    int displs[3] = {0, 1, 3};  // P0 -> start at 0, P1 -> start at 1, P2 -> start at 3

    MPI_Scatterv(sendbuf, sendcounts, displs, MPI_INT, recvbuf, sendcounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d received:", rank);
    for (int i = 0; i < sendcounts[rank]; i++) {
        printf(" %d", recvbuf[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}
