#include <stdio.h>
#include <omp.h>
#include <semaphore.h>

sem_t semA,semB;

void processA(){
    printf("Process A has arrived!");
    sem_post(&semA);
    sem_wait(&semB);
    printf("Process A leaves after rendez-vous");
}


void processB(){
    printf("Process B has arrived!");
    sem_post(&semB);
    sem_wait(&semA);
    printf("Process B leaves after rendez-vous");
}

int main(){
    sem_init(&semA,0,0);
    sem_init(&semB,0,0);
    omp_set_num_threads(2);
    #pragma omp parallel 
    {
        #pragma omp single
        {
            #pragma omp task
            {
                processA();
            }
            #pragma omp task
            {
                processB();
            }
        }
    }

    /*
    or
    int main(){
    sem_init(&semA,0,0);
    sem_init(&semB,0,0);
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            processA();
        }
        #pragma omp section
        {
            processB();
        }
    }
}
    */
}