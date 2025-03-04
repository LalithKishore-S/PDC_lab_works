#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int n, r;
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

    int *arr = (int*)malloc(8 * sizeof(int));
    int *recv = (int*)malloc(2 * sizeof(int));

    if (r == 0) {
        srand(time(NULL));
        for (int i = 0; i < 8; i++) 
            arr[i] = (rand() / (double)RAND_MAX) >= 0.5 ? 1 : 0;

        printf("Initial array:\n");
        for (int i = 0; i < 8; i++) 
            printf("%d ", arr[i]);
        printf("\nArray scattered across various processes..\n");
    }

    MPI_Scatter(arr, 2, MPI_INT, recv, 2, MPI_INT, 0, MPI_COMM_WORLD);

    int par_sum = 0;
    for (int i = 0; i < 2; i++) 
        par_sum += recv[i];

    int total_sum = 0;
    MPI_Reduce(&par_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (r == 0) {
        printf("Final sum = %d\n", total_sum);
    }

    free(arr);
    free(recv);
    
    MPI_Finalize();
    return 0;
}
