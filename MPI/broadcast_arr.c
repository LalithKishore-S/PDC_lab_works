#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc,char ** argv){
	MPI_Init(&argc,&argv);
	int n,r;
	int * arr = (int *)malloc(5*sizeof(int));

	MPI_Comm_size(MPI_COMM_WORLD,&n);
	MPI_Comm_rank(MPI_COMM_WORLD,&r);

	if(r==0){
		for(int i=0;i<5;i++) arr[i] = i+1;
		printf("Array broadcasted to all processes.\n");
	}

	MPI_Bcast(arr,5,MPI_INT,0,MPI_COMM_WORLD);

	printf("Array at process %d\n",r);
	for(int i=0;i<5;i++) printf("%d ",arr[i]);
	printf("\n");

	MPI_Finalize();
}
