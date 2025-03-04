#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm intercomm;
    char port_name[MPI_MAX_PORT_NAME];

    // Read port name from file
    FILE *fp = fopen("port_name.txt", "r");
    if (fp == NULL) {
        printf("Client: Could not open port_name.txt. Is the server running?\n");
        MPI_Finalize();
        return 1;
    }
    fscanf(fp, "%s", port_name);
    fclose(fp);

    printf("Client: Connecting to server at port %s...\n", port_name);

    // Connect to the server
    MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm);
    printf("Client: Connected to server.\n");

    char message[] = "Hello Server, this is Client!";
    char buffer[100];
    MPI_Status status;

    // Send message to server
    MPI_Send(message, 100, MPI_CHAR, 0, 0, intercomm);
    printf("Client: Sent message to server.\n");

    // Receive response from server
    MPI_Recv(buffer, 100, MPI_CHAR, 0, 0, intercomm, &status);
    printf("Client received: %s\n", buffer);

    MPI_Finalize();
    return 0;
}
