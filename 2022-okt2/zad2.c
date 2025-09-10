#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc,char* argv[]){

    int pid;

    pid=fork();

    if(pid==0){

        char* program=argv[1];

        char** childArgs=malloc((argc)*sizeof(char *));

        childArgs[0]=program;

        for(int i=1;i<argc-1;i++){

            childArgs[i]=argv[argc-i];
        }

        childArgs[argc-1]=NULL;

        execv(program,childArgs);
    }
    else{
        int status;

        wait(&status);

        if(WIFEXITED(status)){

            printf("%d",WEXITSTATUS(status));
        }
        else printf("Greska");

    }

    return 0;
}
