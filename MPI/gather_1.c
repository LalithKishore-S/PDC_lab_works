#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc,char ** argv){
	MPI_Init(&argc,&argv);

	int n,r;
	int * arr = (int *)malloc(4*sizeof(int));

	MPI_Comm_size(MPI_COMM_WORLD,&n);
	MPI_Comm_rank(MPI_COMM_WORLD,&r);

	if(r==0){
		for(int i=0;i<4;i++) arr[i] = 0;
		printf("Initial data\n");
		for(int i=0;i<4;i++) printf("%d ",arr[i]);
		printf("\n");
	}

	int data = r+1;

	MPI_Gather(&data,1,MPI_INT,arr,1,MPI_INT,0,MPI_COMM_WORLD);

	if(r==0){
		printf("Data gathered at process = 0\n");
		for(int i=0;i<4;i++) printf("%d ",arr[i]);
		printf("\n");
	}

	MPI_Finalize();
}
