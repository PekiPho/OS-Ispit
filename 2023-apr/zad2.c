#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc,char* argv[]){

    int pid;
    int pipe1[2],pipe2[2];

    if(pipe(pipe1)==-1){

        perror("Greska pipe1");
        exit(1);
    }

    if(pipe(pipe2)==-1){

        perror("Greska pipe2");
        exit(1);
    }

    pid=fork();
    if(pid==0){
        execl(argv[1],argv[1],argv[2],NULL);

    }
    else{
        int status;
        wait(&status);

        if(WIFEXITED(status)){
            printf("%d\n", WEXITSTATUS(status));

        }
        else printf("Abnormalno mala greska");
    }
}
