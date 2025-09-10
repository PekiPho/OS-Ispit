#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semfull,semempty;

int curr=1;
int N;

void* deljivi(void* arg){

    while(1){
        sem_wait(&semfull);

        if(curr>N){
            sem_post(&semempty);
            break;
        }

        printf("Deljiv: %d \n",curr);

        curr++;

        if(curr%7==0){

            sem_post(&semfull);
        }
        else sem_post(&semempty);

    }
}

void* ostali(void * arg){

    while(1){

        sem_wait(&semempty);

        if(curr>N){
            sem_post(&semfull);
            break;
        }

        printf("%d \n",curr);

        curr++;

        if(curr%7==0){

            sem_post(&semfull);
        }
        else sem_post(&semempty);
    }
}

int main(int argc,char* argv[]){

    N=atoi(argv[1]);

    pthread_t nit1,nit2;

    sem_init(&semfull,0,0);
    sem_init(&semempty,0,1);


    pthread_create(&nit1,NULL,(void *)deljivi,NULL);
    pthread_create(&nit2,NULL,(void *)ostali,NULL);

    pthread_join(nit1,NULL);
    pthread_join(nit2,NULL);

    sem_destroy(&semfull);
    sem_destroy(&semempty);

    return 0;

}
