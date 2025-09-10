#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>


void* prva(void* arg){

    int num=*(int*) arg;
    free(arg);
    int i=num;

    while(i>0){

        printf("%d \n",i--);
        fflush(stdout);
        sleep(1);
    }
}


int main(int argc,char* argv[]){


    pthread_t  nit1;


    char buff[512];
    while(1){
        printf("Unesi broj: ");
        scanf("%s",&buff);

        if(strcmp(buff,"KRAJ")==0)
            break;

        int num=atoi(buff);
        int *arg=malloc(sizeof(int));
        *arg=num;
        pthread_create(&nit1,NULL,prva,arg);

        pthread_join(nit1,NULL);

    }

}
