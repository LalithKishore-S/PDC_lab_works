#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc,char **argv){
	MPI_Init(&argc,&argv);
	
	int n,r;
	MPI_Comm_size(MPI_COMM_WORLD,&n);
	MPI_Comm_rank(MPI_COMM_WORLD,&r);
	
	int * arr = (int *)malloc(n*(n+1)/2*sizeof(int));
	int * recv = (int *) malloc((r+1)*sizeof(int));
	int * send_count = (int *)malloc(n*sizeof(int));
	int * dis = (int *)malloc(n*sizeof(int));

	if(r==0){
		dis[0] = 0;
		for(int i=0;i<n;i++){
			send_count[i] = i+1;
			if(i!=0) dis[i] = dis[i-1] + send_count[i-1];
		}		

		for(int i=0;i<n;i++){
			for(int j=0;j<send_count[i];j++)
				arr[dis[i]+j] = i+1;
		}
		
		printf("Initial array :\n");
		for(int i=0;i<n;i++){
			for(int j=0;j<send_count[i];j++)
				printf("%d ",arr[dis[i]+j]);
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Bcast(send_count,n,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(dis,n,MPI_INT,0,MPI_COMM_WORLD);

	MPI_Scatterv(arr,send_count,dis,MPI_INT,recv,send_count[r],MPI_INT,0,MPI_COMM_WORLD);

	for(int i=0;i<send_count[r];i++) recv[i] *= 10;

	MPI_Allgatherv(recv,send_count[r],MPI_INT,arr,send_count,dis,MPI_INT,MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	printf("Process %d : ",r);
	for(int i=0;i<n*(n+1)/2;i++) printf("%d ",arr[i]);
	printf("\n");

	MPI_Finalize();
}
