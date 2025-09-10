#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){

    int pid;
    int pipe1[2];


    if(pipe(pipe1)==-1){

        perror("Greska pipe1");
        exit(1);
    }


    pid=fork();

    if(pid==0){

        close(pipe1[1]);

        char path[250]={0},keyword[250]={0};

        read(pipe1[0],path,sizeof(path));
        read(pipe1[0],keyword,sizeof(keyword));

        //printf("%s \n %s \n",path,keyword);


        FILE *f=fopen(path,"r");

        if(!f){

            perror("Greska u otvaranju fajla");
            exit(1);
        }

        int line=1;

        char s[500];
        while(fgets(s,500,f)){

            if(strstr(s,keyword)!=NULL){

                printf("%d \n",line);
                fflush(stdout);
            }

            line++;
        }

        close(pipe1[0]);

    }
    else{
        close(pipe1[0]);

        char path[250]={0},keyword[250]={0};

        printf("Unesi putanju: ");
        scanf("%s",path);

        printf("Unesi rec: ");
        scanf("%s",keyword);

        //printf("%s \n",keyword);

        write(pipe1[1],path,sizeof(path));
        write(pipe1[1],keyword,sizeof(keyword));

        close(pipe1[1]);
        wait(NULL);
    }

    return 0;
}
