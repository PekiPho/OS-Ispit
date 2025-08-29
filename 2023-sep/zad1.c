#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int buff[10];
sem_t sem_empty,sem_full;

void* generisi(void *arg){
    while(1){

        sem_wait(&sem_empty);
        for(int i=0;i<10;i++){
            buff[i]=rand()%200;
        }
        sem_post(&sem_full);
    }


}

int main(int argc,char* argv[]){
    srand(time(NULL));

    pthread_t nit1;
    int sum=0;

    sem_init(&sem_empty,0,0);
    sem_init(&sem_full,0,1);
    pthread_create(&nit1,NULL,(void*)generisi,NULL);

    while(1){
        sem_wait(&sem_full);

        for(int i=0;i<10;i++){
            sum+=buff[i];
        }

        if(sum>800){

            printf("Suma veca od 800: %d \n",sum);
            fflush(stdout);
        }
        if(sum > 1300)
            break;

        sum=0;

        sem_post(&sem_empty);

    }

    pthread_cancel(nit1);
    pthread_join(nit1,NULL);

    sem_destroy(&sem_full);
    sem_destroy(&sem_empty);

    return 0;
}
