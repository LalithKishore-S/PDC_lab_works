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
                printf("Initial data\n");
                for(int i=0;i<6;i++) printf("%d ",send[i]);
                printf("\n");
        }

	MPI_Scatter(send,2,MPI_INT,recv,2,MPI_INT,0,MPI_COMM_WORLD);

        for(int i=0;i<2;i++) recv[i] *= recv[i];

        MPI_Gather(recv,2,MPI_INT,send,2,MPI_INT,0,MPI_COMM_WORLD);

        if(r==0){
                printf("Data gathered at process = 0\n");
                for(int i=0;i<6;i++) printf("%d ",send[i]);
                printf("\n");
        }

        MPI_Finalize();
}
