#include<iostream>
#include<mpi.h>
using namespace std;

int main(int argc,char ** argv){
	MPI_Init(&argc,&argv);
	int n,x;
	MPI_Comm_size(MPI_COMM_WORLD,&n);
	MPI_Comm_rank(MPI_COMM_WORLD,&x);
	cout<<"Hello world from process "<<x<<" out of "<<n<<" processes\n";
	MPI_Finalize();
}
