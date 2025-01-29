#include <stdio.h>
#include<omp.h>

int main(){
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        printf("%d :Hello\n",omp_get_thread_num()) ;
        printf("%d :World\n",omp_get_thread_num()) ;
    }
}

/* Output :
0 :Hello
0 :World
1 :Hello
1 :World
3 :Hello
3 :World
2 :Hello
2 :World
*/