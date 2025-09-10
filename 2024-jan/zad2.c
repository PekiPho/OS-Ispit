#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

int odrediSumu(char rec[]){
    int suma=0;
    int num=0;
    int in=0;

    int i=0;
    while(rec[i]!='\0'){

        if(isdigit(rec[i])){

            num=num*10+rec[i]-'0';
            in=1;
        }
        else{

            if(in){

                suma+=num;
                num=0;
                in=0;
            }
        }

        i++;
    }

    if(in) suma+=num;

    return suma;

}


int main(){


    char recenica[1000];

    printf("Unesi recenicu: ");
    fgets(recenica,sizeof(recenica),stdin);
    recenica[strlen(recenica)-1] = 0;

    int pipe1[2],pipe2[2];
    int pid;

    if(pipe(pipe1)==-1){
        perror("Greska pipe1");
        exit(1);
    }

    if(pipe(pipe2)==-1){
        perror("Greska pipe2");
        exit(1);
    }

    pid=fork();
    if(pid>0){
        close(pipe1[1]);
        close(pipe2[0]);

        char rec[100];
        int sum;

        while(read(pipe1[0],rec,sizeof(rec))>0){

            sum=odrediSumu(rec);
            write(pipe2[1],&sum,sizeof(sum));
        }

        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);

    }
    else{
        close(pipe1[0]);
        close(pipe2[1]);

        char* token=strtok(recenica," ");
        int sum;

        while(token){

            write(pipe1[1],token,strlen(token)+1);
            read(pipe2[0],&sum,sizeof(sum));

            printf("Suma u reci %s je: %d \n",token,sum);
            token=strtok(NULL," ");
        }

        close(pipe1[1]);
        close(pipe2[0]);
        wait(NULL);
    }

    return 0;
}
