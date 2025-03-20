#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

int main(int argc,char ** argv){
	MPI_Init(&argc,&argv);

	int n,r;
	MPI_Comm_size(MPI_COMM_WORLD,&n);
	MPI_Comm_rank(MPI_COMM_WORLD,&r);

	int * arr = NULL;
	int * send_count = NULL;
	int * dis = NULL;
	int * recv = NULL;
	int N,k;

	send_count = (int *)malloc(n*sizeof(int));
	dis = (int *)malloc(n*sizeof(int));

	if(r==0){
		printf("Enter the number of elements in array(Mostly multiples of 4):\n");
		scanf("%d",&N);
		arr = (int *)malloc(N*sizeof(int));

		printf("Enter array elements:\n");
		for(int i=0;i<N;i++) scanf("%d",&arr[i]);

		printf("Enter the scalar:\n");
		scanf("%d",&k);

		int base = N/n;
		int remain = N%n;
		for(int i=0;i<n;i++) send_count[i] = (i==n-1)?base+remain:base;
		dis[0] = 0;
		for(int i=1;i<n;i++) dis[i] = dis[i-1] + send_count[i-1];

		printf("Initial array:\n");
		for(int i=0;i<N;i++) printf("%d ",arr[i]);
		printf("\n");
	}
	MPI_Bcast(send_count,n,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(dis,n,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&k,1,MPI_INT,0,MPI_COMM_WORLD);


	recv = (int *)malloc(send_count[r]*sizeof(int));
	MPI_Scatterv(arr,send_count,dis,MPI_INT,recv,send_count[r],MPI_INT,0,MPI_COMM_WORLD);

	for(int i=0;i<send_count[r];i++) recv[i] *= k;

	MPI_Gatherv(recv,send_count[r],MPI_INT,arr,send_count,dis,MPI_INT,0,MPI_COMM_WORLD);

	if(r==0){
		printf("Final array:\n");
		for(int i=0;i<N;i++) printf("%d ",arr[i]);
		printf("\n");
	}

	MPI_Finalize();
}
