#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

FILE *file1;
int niz[200];
int n=0;

void *sortira(void *arg){

    for(int i=0;i<n-1;i++){

        for(int j=i+1;j<n;j++){

            if(niz[i]>niz[j]){

                int temp=niz[i];
                niz[i]=niz[j];
                niz[j]=temp;
            }
        }
    }
}

int main(int argc,char *argv[]){

    pthread_t nit1;

    file1=fopen("ulaz.txt","r");

    while(fscanf(file1,"%d",&niz[n])==1){
        n++;
    }

    fclose(file1);

    pthread_create(&nit1,NULL,(void*)sortira,NULL);

    pthread_join(nit1,NULL);

    for(int i=0;i<n;i++){
        printf("%d ",niz[i]);
    }
    printf("\n");

    return 0;
}
