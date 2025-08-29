#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int buff[10];
void *generisi(void* arg){

    int smer = *(int *)arg;
    int count=0;
    int i=smer;
    while(count < 5){

        if(smer==0){

            buff[i]=rand()%300;
        }
        if(smer==1){

            buff[i]=(rand()%200)+300;
        }
        i+=2;
        count++;
    }
}

int main(int argc,char *argv[]){
    srand(time(NULL));

    pthread_t nit1,nit2;
    int smer1=1,smer2=0;

    int j=0;
    while(j<5){

    if(pthread_create(&nit1,NULL,(void *)generisi,&smer1)!=0){

        perror("Greska kod nit1");
        return 1;
    }

    if(pthread_create(&nit2,NULL,(void *)generisi,&smer2)!=0){

        perror("Greska kod nit2");
        return 1;
    }

    pthread_join(nit1,NULL);
    pthread_join(nit2,NULL);

    int sum=0;
    for(int i=0;i<10;i++){
        sum+=buff[i];
    }

    if(sum>=2000) printf("Suma veca %d\n",sum);
    else printf("Suma manja %d \n",sum);

    j++;
    }

}
