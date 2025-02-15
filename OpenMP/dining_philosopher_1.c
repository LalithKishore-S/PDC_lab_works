#include <stdio.h>
#include <omp.h>
#include <semaphore.h>
#include <windows.h>
#include <stdlib.h>
#include <unistd.h>

#define n 5
omp_lock_t forks[n];

void think(int i){
    printf("Philosopher %d is thinking...\n",i);
    sleep(2);
}

void eat(int i){
    printf("Philosopher %d is hungry...\n",i);
    omp_set_lock(&forks[i]);
    printf("Philosopher %d picks up his left fork..\n",i);
    omp_set_lock(&forks[(i+1)%n]);
    printf("Philosopher %d is eating..\n",i);
    sleep(3);
    printf("Philosopher %d puts forks down..\n",i);
    omp_unset_lock(&forks[(i+1)%n]);
    omp_unset_lock(&forks[i]);
}

void philosophise(int i){
    for(int j=0;j<2;j++){
        think(i);
        eat(i);
    }
}

int main(){
    #pragma omp parallel for
    for(int i=0;i<5;i++) omp_init_lock(&forks[i]);

    #pragma omp parallel for
    for(int i=0;i<n;i++) philosophise(i);
}