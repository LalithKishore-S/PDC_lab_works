#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#define N 3
void printmatrix(int ** a){
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++) printf("%d ",a[i][j]);
		printf("\n");
	}
	printf("\n");
}

void flatten(int *a,int **A){
	int ind =0;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++) a[ind++] = A[i][j];
	}
}
int main(int argc,char **argv){
	MPI_Init(&argc,&argv);
	
	int n,r;
	MPI_Comm_size(MPI_COMM_WORLD,&n);
	MPI_Comm_rank(MPI_COMM_WORLD,&r);

	int **A = (int **)malloc(N*sizeof(int));
	for(int i=0;i<N;i++) A[i] = (int *)malloc(N*sizeof(int));
	int **B = (int **)malloc(N*sizeof(int));
	for(int i=0;i<N;i++) B[i] = (int *)malloc(N*sizeof(int));

	int *a = (int *)malloc(N*N*sizeof(int));
	int *b = (int *)malloc(N*N*sizeof(int));
	
	srand(time(NULL));
	if(r==0){
		for(int i=0;i<N;i++){
			for(int j=0;j<N;j++){
				A[i][j] = rand()%2;
				B[i][j] = rand()%2;
			}
		}
		printf("Matrix A:\n");
		printmatrix(A);
		printf("Matrix B:\n");
		printmatrix(B);

		flatten(a,A);
		flatten(b,B);
	}
	MPI_Bcast(a,N*N,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(b,N*N,MPI_INT,0,MPI_COMM_WORLD);

	int * res = (int *)malloc(N*sizeof(int));

	int chunk = N;
	int start = r*chunk;
	int end = start+chunk;

	for(int i=0;i<N;i++){
		res[i] = 0;
		for(int j =start;j<end;j++){
			res[i] += a[j]*b[i+N*(j-start)];
		}
	}

	MPI_Gather(res,N,MPI_INT,a,N,MPI_INT,0,MPI_COMM_WORLD);

	if(r==0){
		int ind = 0;
		for(int i=0;i<N;i++){
			for(int j=0;j<N;j++) A[i][j] = a[ind++];
		}
		printf("Result :\n");
		printmatrix(A);
	}
	MPI_Finalize();
}
