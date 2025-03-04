#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    MPI_Comm intercomm;
    int n, r;

    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

    char port_name[MPI_MAX_PORT_NAME];

    int size;
    int *arr = NULL;

    if (r == 0) {
        // Open a communication port
        MPI_Open_port(MPI_INFO_NULL, port_name);

        // Save the port name to a file
        FILE *fp = fopen("port.txt", "w");
        if (fp) {
            fprintf(fp, "%s", port_name);
            fclose(fp);
        } else {
            printf("Error: Cannot open port file for writing.\n");
            MPI_Finalize();
            return 1;
        }

        printf("Server running at port %s\n", port_name);

        // Accept client connection
        MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm);
        printf("Client connected\n");

        // Receive the size of the array
        MPI_Recv(&size, 1, MPI_INT, MPI_ANY_SOURCE, 0, intercomm, MPI_STATUS_IGNORE);

        // Allocate memory for array
        arr = (int *)malloc(size * sizeof(int));
        if (arr == NULL) {
            printf("Memory allocation failed\n");
            MPI_Finalize();
            return 1;
        }

        // Receive the array
        MPI_Recv(arr, size, MPI_INT, MPI_ANY_SOURCE, 0, intercomm, MPI_STATUS_IGNORE);
        printf("Received array:\n");
        for (int i = 0; i < size; i++) printf("%d ", arr[i]);
        printf("\n");
    }

    // Broadcast size to all processes
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Allocate memory for the received sub-array
    int *recv = (int *)malloc(size / 4 * sizeof(int));
    MPI_Scatter(arr, size / 4, MPI_INT, recv, size / 4, MPI_INT, 0, MPI_COMM_WORLD);

    // Compute partial sum
    int par_sum = 0;
    for (int i = 0; i < size / 4; i++) par_sum += recv[i];

    // Reduce partial sums to compute total sum
    int sum = 0;
    MPI_Reduce(&par_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Send the sum to the client
    if (r == 0) {
        MPI_Send(&sum, 1, MPI_INT, 0, 0, intercomm);
    }

    // Free allocated memory
    free(recv);
    if (r == 0) free(arr);

    MPI_Finalize();
    return 0;
}
