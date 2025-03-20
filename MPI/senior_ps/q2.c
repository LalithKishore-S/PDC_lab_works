#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc,char ** argv){
	MPI_Init(&argc,&argv);

	int n,r;
	MPI_Comm_size(MPI_COMM_WORLD,&n);
	MPI_Comm_rank(MPI_COMM_WORLD,&r);

	int * vector_1 = (int *)malloc(100*sizeof(int));
	int * vector_2 = (int *)malloc(100*sizeof(int));
	int * vector_3 = (int *)malloc(100*sizeof(int));
	int * recv = NULL;
	if(r==0){
		vector_1[0] = 1;
		vector_1[1] = 1;
		for(int i=2;i<100;i++) vector_1[i] = vector_1[i-1] + vector_1[i-2];
	}

	int base = 100/n;
	recv = (int *)malloc(base*sizeof(int));
	for(int i=0;i<base;i++) recv[i] = r*base+i;
	
	MPI_Allgather(recv,base,MPI_INT,vector_2,base,MPI_INT,MPI_COMM_WORLD);
	MPI_Bcast(vector_1,100,MPI_INT,0,MPI_COMM_WORLD);

	for(int i=0;i<base;i++) recv[i] = vector_1[r*base+i] + vector_2[r*base+i];

	MPI_Gather(recv,base,MPI_INT,vector_3,base,MPI_INT,0,MPI_COMM_WORLD);

	MPI_Scatter(vector_3,base,MPI_INT,recv,base,MPI_INT,0,MPI_COMM_WORLD);
	long long int sum;
	long long int par_sum = 0;
	for(int i=0;i<base;i++) par_sum += recv[i];

	MPI_Reduce(&par_sum,&sum,1,MPI_LONG_LONG_INT,MPI_SUM,0,MPI_COMM_WORLD);

	if(r==0){
		printf("Final sum = %llu\n",sum);
	}
	MPI_Finalize();
}
