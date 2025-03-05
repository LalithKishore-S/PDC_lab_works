#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define N 8  // Must be multiple of 4

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int arr[N];
    MPI_Comm intercomm;
    char port_name[MPI_MAX_PORT_NAME];

    // Get array input from user
    printf("Enter %d array elements:\n", N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &arr[i]);
    }

    // Read server port from file
    FILE *fp = fopen("port.txt", "r");
    if (fp == NULL) {
        printf("Error: Could not open port.txt\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    fscanf(fp, "%s", port_name);
    fclose(fp);
    printf("Read port: %s\n", port_name);

    // Connect to server
    MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm);
    printf("Connected to server.\n");

    // Send array to server
    MPI_Send(arr, N, MPI_INT, 0, 0, intercomm);
    printf("Array sent to server.\n");

    // Receive final sum from server
    int final_sum;
    MPI_Recv(&final_sum, 1, MPI_INT, MPI_ANY_SOURCE, 0, intercomm, MPI_STATUS_IGNORE);
    printf("Final sum received from server: %d\n", final_sum);

    MPI_Finalize();
    return 0;
}
