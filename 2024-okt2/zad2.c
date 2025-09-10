#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){



    int pid;
    int pipe1[2],pipe2[2];

    if(pipe(pipe1)==-1){
        perror("pipe");
        exit(1);
    }

    if(pipe(pipe2)==-1){
        perror("pipe");
        exit(1);
    }

    pid=fork();
    if(pid>0){
        close(pipe1[1]);
        close(pipe2[0]);

        char path[250],keyword[250];

        read(pipe1[0],path,sizeof(path));
        read(pipe1[0],keyword,sizeof(keyword));

        FILE *f=fopen(path,"r");

        char line[500];

        int no=0;

        while(fgets(line,sizeof(line),f)){
            no++;
            if(strstr(line,keyword)!=NULL){
                write(pipe2[1],&no,sizeof(int));

            }

        }

    }
    else{
        close(pipe1[0]);
        close(pipe2[1]);

        char path[250],keyword[250];

        printf("Unesi putanju: ");
        scanf("%s",path);


        printf("Unesi rec: ");
        scanf("%s",keyword);

        write(pipe1[1],path,sizeof(path));
        write(pipe1[1],keyword,sizeof(keyword));
        close(pipe1[1]);


        int no;

        while(read(pipe2[0],&no,sizeof(int))>0){

            printf("%d\n",no);
        }

        close(pipe2[0]);
        wait(NULL);
    }


    return 0;
}
