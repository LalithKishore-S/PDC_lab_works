#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc,char ** argv){
	MPI_Init(&argc,&argv);
	int n,r;

	MPI_Comm_size(MPI_COMM_WORLD,&n);
	MPI_Comm_rank(MPI_COMM_WORLD,&r);

	int send = r+1;
	int * recv = (int *)malloc(n*sizeof(int));

	MPI_Allgather(&send,1,MPI_INT,recv,1,MPI_INT,MPI_COMM_WORLD);

	printf("Process %d : ",r);
	for(int i=0;i<n;i++) printf("%d ",recv[i]);
	printf("\n");

	MPI_Finalize();
}
