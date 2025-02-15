#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int * par(int a[],int *n){
    omp_lock_t h_locks[3];
    int *h = (int*)malloc(3*sizeof(int));

    #pragma omp parallel for
    for(int i=0;i<3;i++){
        omp_init_lock(&h_locks[i]);
        h[i] = 0;
    }

    #pragma omp parallel for
    for(int i=0;i<*n;i++){
        omp_set_lock(&h_locks[a[i]-1]);
        h[a[i]-1]++;
        omp_unset_lock(&h_locks[a[i]-1]);
    }

    return h;
}
int main(){
    int * a = (int *) malloc(10*sizeof(int));
    int n = 10;
    for(int i=0;i<10;i++) scanf("%d",a+i);

    double start = omp_get_wtime();
    int * h = par(a,&n);
    double end = omp_get_wtime();
    printf("Par execution time : %f\n",end-start);
    for(int i=0;i<3;i++) printf("%d ",*(h+i));
}