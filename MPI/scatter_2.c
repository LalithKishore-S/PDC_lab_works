#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc,char ** argv){
	MPI_Init(&argc,&argv);
	int n,r;
	int * send = (int *)malloc(6*sizeof(int));
	int * recv = (int *)malloc(2*sizeof(int));

	MPI_Comm_size(MPI_COMM_WORLD,&n);
	MPI_Comm_rank(MPI_COMM_WORLD,&r);

	if(r==0){
		for(int i=0;i<6;i++) send[i] = i+1;
		printf("Array scattered among the processes:\n");
	}
	MPI_Scatter(send,2,MPI_INT,recv,2,MPI_INT,0,MPI_COMM_WORLD);

	recv[0] *= 2;
	recv[1] *= 2;

	printf("Process %d received: %d %d\n", r, recv[0], recv[1]);

	MPI_Finalize();
}
