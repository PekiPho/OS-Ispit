#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc,char* argv[]){

    int pid,pid2;
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
    if(pid<0){

        perror("Greska pid");
        exit(1);
    }

    if(pid ==0){

        close(pipe1[1]);
        close(pipe2[0]);

        char path[250],keyword[250];


        read(pipe1[0],path,sizeof(path));
        read(pipe1[0],keyword,sizeof(keyword));

        //printf("%s \n %s \n",path,keyword);

        FILE *f=fopen(path,"r");

        char s[500];
        int niz[100];
        int i=0,no=1;
        while(fgets(s,sizeof(s),f)){

            if(strstr(s,keyword)!=NULL){
                //printf("Hit %d",no);
                niz[i++]=no;
            }

            no++;
        }
        fclose(f);

        //printf("\n %d \n",i);

        write(pipe2[1],&i,sizeof(int));
        write(pipe2[1],niz,i*sizeof(int));



        close(pipe1[0]);
        close(pipe2[1]);

        exit(0);
    }

    pid2=fork();

    if(pid2<0){

        perror("Greska pid2");
        exit(1);
    }

    if(pid2 ==0){

        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);

        int i,niz[100];
        read(pipe2[0],&i,sizeof(int));
        read(pipe2[0],niz,i*sizeof(int));

        //printf("%d \n", i);

        int suma=0;
        for(int k=0;k<i;k++){
            //printf("%d ",niz[k]);
            suma+=niz[k];
        }

        printf("%d \n",suma);
        fflush(stdout);

        close(pipe2[0]);

        exit(0);
    }


    close(pipe1[0]);
    close(pipe2[0]);
    close(pipe2[1]);

    char path[250],keyword[250];

    printf("Unesi naziv txt fajla: ");
    scanf("%s",path);

    printf("Unesi kljucnu rec: ");
    scanf("%s",keyword);

    write(pipe1[1],path,sizeof(path));
    write(pipe1[1],keyword,sizeof(keyword));

    close(pipe1[1]);

    waitpid(pid,NULL,0);
    waitpid(pid2,NULL,0);

    return 0;
}
