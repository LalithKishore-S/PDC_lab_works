#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sendbuf[6] = {1, 2, 3, 4, 5, 6};  // Data in root process
    int recvbuf[3]; // Buffer for receiving scattered data (max needed is 3)

    // Define how many elements each process gets
    int sendcounts[3] = {1, 2, 3};  // P0 -> 1, P1 -> 2, P2 -> 3

    // Define the displacement (starting index for each process)
    int displs[3] = {0, 1, 3};  // P0 -> start at 0, P1 -> start at 1, P2 -> start at 3

    // Scatter different numbers of elements to different processes
    MPI_Scatterv(sendbuf, sendcounts, displs, MPI_INT, recvbuf, sendcounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    // Modify received data (each process doubles its received values)
    for (int i = 0; i < sendcounts[rank]; i++) {
        recvbuf[i] *= 2;
    }

    // Gather modified data back to the root process
    MPI_Gatherv(recvbuf, sendcounts[rank], MPI_INT, sendbuf, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    // Print modified data in root process
    if (rank == 0) {
        printf("Modified array after gathering:\n");
        for (int i = 0; i < 6; i++) {
            printf("%d ", sendbuf[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
