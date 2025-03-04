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

    // Read the port from file
    FILE *fp = fopen("port.txt", "r");
    if (fp == NULL) {
        printf("Error: Cannot open port file\n");
        MPI_Finalize();
        return 1;
    }
    fscanf(fp, "%s", port_name);
    fclose(fp);

    int *arr = NULL;
    int size;

    if (r == 0) {
        printf("Enter the size of the array (must be multiple of 4): ");
        scanf("%d", &size);

        // Allocate memory for the array
        arr = (int*)malloc(size * sizeof(int));
        if (arr == NULL) {
            printf("Memory allocation failed\n");
            MPI_Finalize();
            return 1;
        }

        printf("Enter the array elements:\n");
        for (int i = 0; i < size; i++) {
            scanf("%d", &arr[i]);
        }

        // Connect to server
        MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm);
	printf("Connected\n");

        // Send the array size and elements
        MPI_Send(&size, 1, MPI_INT, 0, 0, intercomm);
        MPI_Send(arr, size, MPI_INT, 0, 0, intercomm);

        // Receive the sum from the server
        int sum = 0;
        MPI_Recv(&sum, 1, MPI_INT, 0, 0, intercomm, MPI_STATUS_IGNORE);
        printf("Sum = %d\n", sum);
    }

    // Free allocated memory
    if (r == 0) free(arr);

    MPI_Finalize();
    return 0;
}
