#include <stdio.h>
#include <mpi.h>

int main(int argc,char **argv){
	MPI_Init(&argc,&argv);
	int n,r,data;

	MPI_Comm_size(MPI_COMM_WORLD,&n);
	MPI_Comm_rank(MPI_COMM_WORLD,&r);

	if(r==0){
		data = 100;
		printf("Data broadcasted from master process\n");
	}
	MPI_Bcast(&data,1,MPI_INT,0,MPI_COMM_WORLD);
	printf("Data = %d at process = %d\n",data,r);
	MPI_Finalize();
}
