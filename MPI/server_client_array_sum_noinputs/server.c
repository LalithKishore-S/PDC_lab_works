#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define N 8  // Must be multiple of 4

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm intercomm;
    int *arr = NULL;
    int local_sum = 0, final_sum = 0;
    char port_name[MPI_MAX_PORT_NAME];

    if (rank == 0) {
        // Step 1: Open port and listen for client
        MPI_Open_port(MPI_INFO_NULL, port_name);
        printf("Server listening at port: %s\n", port_name);

        // Write port name to file so client can read it
        FILE *fp = fopen("port.txt", "w");
        if (fp == NULL) {
            printf("Error: Unable to write to port.txt\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fprintf(fp, "%s", port_name);
        fclose(fp);
        printf("Waiting for client connection...\n");

        // Accept client connection
        MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm);
        printf("Client connected!\n");

        // Step 2: Receive array from client
        arr = (int *)malloc(N * sizeof(int));
        MPI_Recv(arr, N, MPI_INT, MPI_ANY_SOURCE, 0, intercomm, MPI_STATUS_IGNORE);
        printf("Received array: ");
        for (int i = 0; i < N; i++) printf("%d ", arr[i]);
        printf("\n");

        // Step 3: Scatter array to 4 processes
        int sub_array_size = N / 4;
        int local_arr[sub_array_size];

        MPI_Scatter(arr, sub_array_size, MPI_INT, local_arr, sub_array_size, MPI_INT, 0, MPI_COMM_WORLD);

        // Step 4: Compute partial sum
        for (int i = 0; i < sub_array_size; i++) {
            local_sum += local_arr[i];
        }
    }

    // Step 5: Reduce to get final sum
    MPI_Reduce(&local_sum, &final_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Step 6: Send sum back to client
    if (rank == 0) {
        printf("Final sum: %d\n", final_sum);
        MPI_Send(&final_sum, 1, MPI_INT, 0, 0, intercomm);
        free(arr);
    }

    MPI_Finalize();
    return 0;
}
