#include <stdio.h>
#include <omp.h>

int main(){
    int n;
    printf("Enter value of n :");
    scanf("%d",&n);

    double start = omp_get_wtime();
    int sum=0;
    for(int i=1;i<=n;i++) sum += i;
    double end = omp_get_wtime();
    printf("Sequential\n");
    printf("Value = %d\nTime taken = %f\n",sum,end-start);

    start = omp_get_wtime();
    sum=0;
    
    omp_set_num_threads(5);
    #pragma omp parallel for reduction(+:sum)
    for(int i=1;i<=n;i++) sum += i;
    end = omp_get_wtime();
    printf("Parallel\n");
    printf("Value = %d\nTime taken = %f\n",sum,end-start);
}