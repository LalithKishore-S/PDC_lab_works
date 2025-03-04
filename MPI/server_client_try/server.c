#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm intercomm;
    char port_name[MPI_MAX_PORT_NAME];

    // Open a port for client connection
    MPI_Open_port(MPI_INFO_NULL, port_name);
    printf("Server: Port opened at %s\n", port_name);

    // Write port name to a file so the client can read it
    FILE *fp = fopen("port_name.txt", "w");
    if (fp == NULL) {
        printf("Error opening file to store port name.\n");
        MPI_Finalize();
        return 1;
    }
    fprintf(fp, "%s", port_name);
    fclose(fp);
    printf("Server: Waiting for client to connect...\n");

    // Accept connection from client
    MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm);
    printf("Server: Client connected.\n");

    char buffer[100];
    MPI_Status status;

    // Receive message from client
    MPI_Recv(buffer, 100, MPI_CHAR, MPI_ANY_SOURCE, 0, intercomm, &status);
    printf("Server received: %s\n", buffer);

    // Send response to client
    char response[] = "Hello from Server!";
    MPI_Send(response, 100, MPI_CHAR, 0, 0, intercomm);
    printf("Server: Response sent to client.\n");

    // Close port
    MPI_Close_port(port_name);
    MPI_Finalize();
    return 0;
}
