#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int n, r;
    int data;
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

    if (r == 0) {
        data = 2;
        printf("Data = %d sent from process %d to process 1\n", data, r);
        MPI_Ssend(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); // Ensures message is received
        
        MPI_Recv(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Data = %d received from process 1 to process %d\n", data, r);
    } 
    else if (r == 1) {
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Data = %d received from process 0 to process %d\n", data, r);
        
        data *= 2;
        printf("Data = %d sent from process %d to process 0\n", data, r);
        MPI_Ssend(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // Ensures message is received
    }

    MPI_Finalize();
    return 0;
}
