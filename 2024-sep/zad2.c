#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc,char* argv[]){

    int pid;
    int pipe1[2];

    if(pipe(pipe1)==-1){

        perror("Greska pipe1");
        exit(1);
    }

    pid=fork();
    if(pid==0){
        close(pipe1[1]);

        FILE* o=fopen(argv[2],"wb");

        unsigned char key= (rand()%255)+1;

        fwrite(&key,1,1,o);

        int n;
        char buff[1000];

        while((n=read(pipe1[0],buff,1000))>0){

            for(int i=0;i<n;i++){

                buff[i]^=key;
            }

            fwrite(buff,1,n,o);
        }

        fclose(o);
        close(pipe1[0]);

        exit(0);

    }
    else{
        close(pipe1[0]);

        FILE *f=fopen(argv[1],"rb");

        char buff[1000];
        int n=fread(buff,1,1000,f);
        while(n>0){

            write(pipe1[1],buff,n);
            n=fread(buff,1,1000,f);
        }

        fclose(f);
        close(pipe1[1]);

        wait(NULL);
    }
}
