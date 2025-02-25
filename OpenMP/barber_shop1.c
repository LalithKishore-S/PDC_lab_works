#include <stdio.h>
#include <omp.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#define SIZE 5
#define n 6
#define m 2

omp_lock_t chair;
sem_t empty,full,barb,cust;
int buffer[SIZE];
int in = 0;
int out = 0;
int served = 0;

void barber(){
    while(true){
        sem_wait(&full);
        //Customer present so barber invites the customer to chair
        int cus = buffer[out];
        out = (out+1)%SIZE;
        sem_post(&empty);
        printf("Barber invites customer %d to chair...\n",cus);
        sem_post(&barb);
        //Barber waits for customer to come to chair
        sem_wait(&cust);
        //Barber starts his process
        omp_set_lock(&chair);
        //Simulating process
        printf("Customer %d is getting served...\n",cus);
        Sleep(m*1000);

        omp_unset_lock(&chair);
        
        sem_post(&barb);
        //Barber asks customer to leave
        printf("Barber asks customer %d to leave..\n",cus);
        sem_wait(&cust);
        //Barber waits for customer to leave 
        served++;
        if(served>=n) {
            printf("Barber closes the shop for the day.\n");
            return;
        }

    }
}

void customer(int i){
    if(sem_trywait(&empty)==-1){//The sem_trywait() function shall lock the semaphore referenced by sem only if the semaphore is currently not locked; that is, if the semaphore value is currently positive. Otherwise, it shall not lock the semaphore.
        printf("Customer %d sees that waiting line is full so leaves...\n",i);
        //semaphore empty is already decreemented in sem_trywait so need to increement 
        return;
    }
    //Customer enters waiting line
    printf("Customer %d enters the waiting line...",i);
 
    buffer[in] = i;
    in = (in+1)%SIZE;

    sem_post(&full);

    sem_wait(&barb);
    //Customer waits for barber to call him to chair
    //Simulate arrival time to chair
    Sleep(m*500);
    sem_post(&cust);
    //Inform barber that he reached the chair
    sem_wait(&barb);
    //Wait for barber to tell customer to leave
    Sleep(m*500);
    //Simulate leaving time
    sem_post(&cust); 
}

int main(){
    sem_init(&empty,0,SIZE);
    sem_init(&full,0,0);
    sem_init(&barb,0,0);
    sem_init(&cust,0,0);
    omp_init_lock(&chair);

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            barber();
        }
        #pragma omp section
        {
            omp_set_num_threads(5);
            omp_set_nested(1);
            #pragma omp parallel for
            for(int i=1;i<=n;i++){
                customer(i);
                Sleep(500);
            }
        }
    }
    
}