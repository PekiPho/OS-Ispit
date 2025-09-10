#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

FILE *file1,*file2,*output;
pthread_mutex_t file_mutex;
sem_t sem_odd, sem_even;

void *prva(void *arg) { //u arg se salje ime putanje
    int linija=1;
    char* file= (char*)arg;

    char buff[512];

    while(fgets(buff,sizeof(buff),file1)){


        sem_wait(&sem_even);

        pthread_mutex_lock(&file_mutex);

        fprintf(output,"%s %d: %s",arg,linija,buff);

        pthread_mutex_unlock(&file_mutex);

        sem_post(&sem_odd);
        linija++;
    }
}

void *druga(void *arg){
    int linija=1;
    char* file= (char*)arg;

    char buff[512];

    while(fgets(buff,sizeof(buff),file2)){


        sem_wait(&sem_odd);

        pthread_mutex_lock(&file_mutex);

        fprintf(output,"%s %d: %s",arg,linija,buff);

        pthread_mutex_unlock(&file_mutex);

        sem_post(&sem_even);
        linija++;
    }

}


int main(int arc,char *argv[]){

    file1=fopen(argv[1],"r");
    file2=fopen(argv[2],"r");
    output=fopen("zbir.txt","w");

    if(!file1 || !file2 || !output){
        perror("Greska kod otvaranja fajlova");
    }

    sem_init(&sem_even, 0, 1);
    sem_init(&sem_odd, 0, 0);

    pthread_t nit1,nit2;

    pthread_create(&nit1,NULL,(void *)prva,(void *)argv[1]);
    pthread_create(&nit2,NULL,(void *)druga,(void *)argv[2]);

    pthread_join(nit1,NULL);
    pthread_join(nit2,NULL);


    pthread_mutex_destroy(&file_mutex);
    sem_destroy(&sem_even);
    sem_destroy(&sem_odd);

    return 0;
}
