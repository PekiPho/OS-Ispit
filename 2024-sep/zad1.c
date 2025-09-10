#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semempty,semfull;
int broj;

void* funk(void *arg){

    while(1){

        sem_wait(&semfull);

        int num=*(int*)arg;

        if(num==999){
            sem_post(&semempty);
            break;
        }


        while(num>=0){

            printf("%d \n",num);
            num--;
            //sleep(2);

        }
        sem_post(&semempty);
    }


}

int main(int* argc,char* argv[]){

    pthread_t nit1;

    sem_init(&semempty,0,1);
    sem_init(&semfull,0,0);

    int *arg=malloc(sizeof(int));


    pthread_create(&nit1,NULL,(void *)funk,arg);


    while(1){
        sem_wait(&semempty);

        scanf("%d",arg);
        //*arg=broj;
        if(*arg==999){
            sem_post(&semfull);
            break;
        }


        sem_post(&semfull);
    }

    pthread_join(&nit1,NULL);

    sem_destroy(&semempty);
    sem_destroy(&semfull);

    return 0;
}
