#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<time.h>
# define SIZE 8

int main(int argc,char** argv){
	MPI_Init(&argc,&argv);

	int n,r;
	MPI_Comm_size(MPI_COMM_WORLD,&n);
	MPI_Comm_rank(MPI_COMM_WORLD,&r);

	int *a = (int *)malloc(SIZE*sizeof(int));
	int *b = (int *)malloc(SIZE*sizeof(int));

	int *a1 = (int *)malloc(2*sizeof(int));
	int *b1 = (int *)malloc(2*sizeof(int));


	srand(time(NULL));
	if(r==0){
		for(int i=0;i<SIZE;i++){
			a[i] = rand()/(double)RAND_MAX>=0.5?1:0;
			b[i] = rand()/(double)RAND_MAX>=0.5?1:0;
		}
		printf("Vector 1:");
		for(int i=0;i<SIZE;i++) printf("%d ",a[i]);
		printf("\n");
		printf("Vector 2:");
		for(int i=0;i<SIZE;i++) printf("%d ",b[i]);
		printf("\n");
		printf("Scattering arrays to all processes..\n");
	}
	MPI_Scatter(a,2,MPI_INT,a1,2,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(b,2,MPI_INT,b1,2,MPI_INT,0,MPI_COMM_WORLD);
	
	int par_dot = 0;
	for(int i=0;i<2;i++) par_dot += a1[i]*b1[i];

	int dot = 0;
	MPI_Reduce(&par_dot,&dot,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

	if(r==0){
		printf("Dot product = %d\n",dot);
	}

	MPI_Finalize();	
}
