#include <stdio.h>
#include <omp.h>
#include <stdlib.h>


int main(){
    int n = 10000;
    int max = 1;
    int min = -1;
    int m = 0;

    double start = omp_get_wtime();
    for(int i=0;i<n;i++){
        float x = (float)rand() / RAND_MAX * 2.0f - 1.0f;  // Random float in [-1,1]
        float y = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        if(x*x + y*y <=1) m++;
    }
    double end = omp_get_wtime();
    printf("Sequential\n");
    printf("Value = %f\nTime taken = %f\n",4.0f*m/n,end-start);

    m = 0;
    start = omp_get_wtime();
    omp_set_num_threads(5);
    #pragma omp parallel for schedule(static,20)
    for(int i=0;i<n;i++){
        float x = (float)rand() / RAND_MAX * 2.0f - 1.0f;  // Random float in [-1,1]
        float y = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        
        #pragma omp critical
        {
            if(x*x + y*y <=1) m++;
        }
    }
    end = omp_get_wtime();
    printf("Parallel\n");
    printf("Value = %f\nTime taken = %f\n",4.0f*m/n,end-start);

}