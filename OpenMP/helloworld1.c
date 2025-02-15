#include<stdio.h>
#include<omp.h>

int main(){
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        printf("Hello %d\n",id);
        printf("World %d\n",id);
    }
}
/*
Hello 3
World 3
Hello 0
World 0
Hello 2
Hello 1
World 1
Hello 6
World 6
Hello 4
World 4
Hello 7
World 7
World 2
Hello 5
World 5*/