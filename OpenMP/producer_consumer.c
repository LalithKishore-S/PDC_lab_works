#include <stdio.h>
#include <omp.h>
# include <stdlib.h>
#include <semaphore.h>

#define SIZE 5
int buffer[SIZE];
int n=10;
int in = 0;
int out =0;
sem_t sem_full,sem_empty;
omp_lock_t lock;

void producer(){
    for(int i=0;i<n;i++){
        sem_wait(&sem_empty);
        omp_set_lock(&lock);
        int item = rand();
        buffer[in] = item;
        in = (in+1)%SIZE;
        printf("Producer produced item : %d\n",item);
        sem_post(&sem_full);
        omp_unset_lock(&lock);
    }
}

void consumer(){
    for(int i=0;i<n;i++){
        sem_wait(&sem_full);
        omp_set_lock(&lock);
        int item = buffer[out];
        out = (out+1)%SIZE;
        printf("Consumer consumed item : %d\n",item);
        sem_post(&sem_empty);
        omp_unset_lock(&lock);
    }
}

int main(){
    sem_init(&sem_full,0,0);
    sem_init(&sem_empty,0,SIZE);
    omp_init_lock(&lock);

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            producer();
        }
        #pragma omp section
        {
            consumer();
        }
    }
}