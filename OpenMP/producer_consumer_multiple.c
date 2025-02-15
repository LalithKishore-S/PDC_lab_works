#include <stdio.h>
#include <omp.h>
#include <windows.h>
#include <stdbool.h>
#include <semaphore.h>
#include <stdlib.h>
#define SIZE 5
#define n 10

int buffer[SIZE];
sem_t empty,full;
int in = 0,out=0;
omp_lock_t lock;

void producer(int id){
    for(int i=0;i<n;i++){

        sem_wait(&empty);
        omp_set_lock(&lock);

        int item = rand();
        buffer[in] = item;
        printf("Producer %d produced item %d at buffer[%d]\n", id, item, in);
        in = (in+1)%SIZE;

        sem_post(&full);
        omp_unset_lock(&lock);
    }
}

void consumer(int id){
    for(int i=0;i<n;i++){

        sem_wait(&full);
        omp_set_lock(&lock);

        int item = buffer[out];
        printf("Consumer %d consumed item %d from buffer[%d]\n", id, item, out);
        out = (out+1)%SIZE;

        sem_post(&empty);
        omp_unset_lock(&lock);

    }
}

int main(){
    sem_init(&empty,0,SIZE);
    sem_init(&full,0,0);
    omp_init_lock(&lock);

    
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            producer(0);
        }
        #pragma omp section
        {
            producer(1);
        }
        
        #pragma omp section
        {
            consumer(0);
        }
        #pragma omp section
        {
            consumer(1);
        }
    }
}