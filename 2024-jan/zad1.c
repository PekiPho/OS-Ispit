#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

pthread_mutex_t lock;

int niz[50];
int len=0;

sem_t semempty,semfull;

void* suma(void* arg){

    while(1){
        sem_wait(&semempty);

        pthread_mutex_lock(&lock);

        if(len==-1){
            pthread_mutex_unlock(&lock);
            break;
        }

        int sum=0;
        for(int i=0;i<len;i++){

            sum+=niz[i];
        }
        printf("%d\n",sum);
        fflush(stdout);

        pthread_mutex_unlock(&lock);
        sem_post(&semfull);
    }
}

int main(int argc,char* argv[]){
    pthread_mutex_init(&lock,NULL);
    FILE *f=fopen("ulaz.txt","r");

    if(!f){
        perror("Greska");
        return 1;
    }

    pthread_t nit1;

    sem_init(&semempty,0,0);
    sem_init(&semfull,0,1);

    pthread_create(&nit1,NULL,suma,NULL);

    //int niz[50];
    int i=0;


    char s[400];
    while(fgets(s,400,f)){

        s[strcspn(s, "\r\n")] = 0;
        if(s[0]=='\0') continue;

        sem_wait(&semfull);
        pthread_mutex_lock(&lock);

        len=0;

        char* token=strtok(s," ");
        while(token!=NULL){
            niz[len++]=atoi(token);
            token=strtok(NULL," ");
        }

        pthread_mutex_unlock(&lock);
        sem_post(&semempty);
    }

    sem_wait(&semfull);
    pthread_mutex_lock(&lock);
    len=-1;
    pthread_mutex_unlock(&lock);
    sem_post(&semempty);

    fclose(f);
    pthread_join(&nit1,NULL);

    pthread_mutex_destroy(&lock);
    sem_destroy(&semfull);
    sem_destroy(&semempty);

    return 0;

}
