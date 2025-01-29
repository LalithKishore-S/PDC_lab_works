#include <omp.h>
#include <stdio.h>
//Sequential
double seq_pi(){
    int n =10000;
    double sum=0;

    double step = 1.0/n;
    for(int i=0;i<n;i++){
        double x = (i + 0.5)*step;
        sum += 4/(1+x*x);
    }
    return step*sum;
}
//Parallel

double par_pi(){
    int n = 10000;
    double sum=0;
    double step = 1.0/n;
    double summ[5];
     
    omp_set_num_threads(5);
    #pragma omp parallel 
    {
        int id = omp_get_thread_num();
        summ[id] = 0;
        for(int i=id;i<n;i+=5){
            double x = (i + 0.5)*step;
            summ[id] += 4/(1+x*x);
        }
    }
    for(int i=0;i<5;i++) sum +=summ[i];
    return step*sum;
}

double par_pi_critical(){
    int n = 10000;
    double sum=0;
    double step = 1.0/n;
     
    omp_set_num_threads(5);
    #pragma omp parallel for
    for(int i=0;i<n;i++){
        double x = (i + 0.5)*step;
        #pragma omp critical
        {
            sum += 4/(1+x*x);
        }
    }
    return step*sum;
}

double par_pi_without_critical(){
    int n = 10000;
    double sum=0;
    double step = 1.0/n;
    double summ[5];

    omp_set_num_threads(5);
    #pragma omp parallel
    {
        summ[omp_get_thread_num()] = 0;
        #pragma omp for
        for(int i=0;i<n;i++){
            double x = (i + 0.5)*step;
            summ[omp_get_thread_num()] += 4.0/(1+x*x);
        }
    }
    for(int i=0;i<5;i++) sum += summ[i];
    return step*sum;
}

double par_pi_reduction(){
    int n = 10000;
    double sum=0;
    double step = 1.0/n;
     
    omp_set_num_threads(5);
    #pragma omp parallel for reduction(+:sum)
    for(int i=0;i<n;i++){
        double x = (i + 0.5)*step;
        sum += 4/(1+x*x);
    }
    return step*sum;
}

int main(){
    double start = omp_get_wtime();
    double pi = seq_pi();
    double end = omp_get_wtime();
    printf("Sequential execution :\n PI value : %f\n Time taken : %f\n",pi,end-start);

    start = omp_get_wtime();
    pi = par_pi();
    end = omp_get_wtime();
    //printf("Start : %f",start);
    //printf("End : %f",end);
    printf("Parallel execution :\n PI value : %f\n Time taken : %f\n",pi,end-start);

    start = omp_get_wtime();
    pi = par_pi_critical();
    end = omp_get_wtime();
    //printf("Start : %f",start);
    //printf("End : %f",end);
    printf("Parallel execution (critical region):\n PI value : %f\n Time taken : %f\n",pi,end-start);

    start = omp_get_wtime();
    pi = par_pi_without_critical();
    end = omp_get_wtime();
    //printf("Start : %f",start);
    //printf("End : %f",end);
    printf("Parallel execution (w/o critical region) :\n PI value : %f\n Time taken : %f\n",pi,end-start);

    start = omp_get_wtime();
    pi = par_pi_reduction();
    end = omp_get_wtime();
    //printf("Start : %f",start);
    //printf("End : %f",end);
    printf("Parallel execution using reduction:\n PI value : %f\n Time taken : %f",pi,end-start);

}

/*
Sequential execution :
 PI value : 3.141593
 Time taken : 0.000000
Parallel execution :
 PI value : 3.141593
 Time taken : 0.000000
Parallel execution :
 PI value : 3.141593
 Time taken : 0.000000
Parallel execution (w/o critical region) :
 PI value : 3.141593
 Time taken : 0.000000
Parallel execution using reduction:
 PI value : 3.141593
 Time taken : 0.000000
*/