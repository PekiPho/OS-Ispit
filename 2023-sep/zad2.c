#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


int main(int argc,char* argv){

    int pipe1[2];
    int pid;

    if(pipe(pipe1)==-1){
        perror("Greska pipe1");
        exit(1);
    }

    pid=fork();

    if(pid==0){

        close(pipe1[1]);

        FILE *o = fopen(argv[2],"wb");

        char buff[200];

        int n=read(pipe1[0],buff,200);
        while(n>0){

            unsigned char key=(rand()%255)+1;

            fwrite(&key,1,1,o);
            for(int i=0;i<n;i++){

                buff[i]^=key;
            }
            fwrite(&buff,1,n,o);
            n=read(pipe1[0],buff,200);
        }

        fclose(o);
        close(pipe1[0]);
        exit(0);
    }
    else{
        close(pipe1[0]);

        FILE* f=fopen(argv[1],"rb");

        int n;
        char buff[200];
        n=fread(buff,1,200,f);
        while(n>0){

            write(pipe1[1],buff,n);
            n=fread(buff,1,200,f);
        }

        fclose(f);
        close(pipe1[1]);
        wait(NULL);
    }

    return 0;

}
