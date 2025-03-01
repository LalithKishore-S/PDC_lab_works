#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int n, r;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

    int *recv = (int *)malloc((r + 1) * sizeof(int));
    for (int i = 0; i <= r; i++) {
        recv[i] = r + 1;  
    }

    int  send_count[3] = {1,2,3};
    int  dis[3] = {0,1,3};
    int *send_flat = NULL;
    if (r == 0) {
        send_flat = (int *)malloc(n * (n + 1) / 2 * sizeof(int));  
    }
    
    MPI_Gatherv(recv,send_count[r], MPI_INT, send_flat,send_count,dis, MPI_INT, 0, MPI_COMM_WORLD);

    if (r == 0) {
	int index = 0;
        for (int i = 0; i < n; i++) {
            printf("Process %d: ", i);
            for (int j = 0; j <= i; j++) {
                printf("%d ", send_flat[index++]);
            }
            printf("\n");
        }

    }

    
    MPI_Finalize();
    return 0;
}
