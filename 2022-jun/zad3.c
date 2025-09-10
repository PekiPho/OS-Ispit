#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>

struct poruka{

    long type;
    char tekst[128];
};

int suma(const char* broj){

    int zbir=0,i=0;

    while(broj[i]!='\0'){
        if(broj[i]>='0' && broj[i]<='9'){

            zbir+=broj[i]-'0';
        }

        i++;
    }

    return zbir;
}

int main(int argc,char* argv[]){

    int pid;
    int redid;

    redid=msgget(10001,IPC_CREAT | 0666);


    pid=fork();
    if(pid==0){

        struct poruka buff;

        for(int i=0;i<10;i++){

            if(msgrcv(redid,&buff,sizeof(buff.tekst),1,0)==-1){

                perror("msgrcv");
                exit(1);
            }
            int sum=suma(buff.tekst);

            printf("%s %d \n",buff.tekst,sum);
            fflush(stdout);
        }

        exit(0);

    }
    else{
        struct poruka msg;
        msg.type=1;

        for(int i=0;i<10;i++){

            printf("Unesi %d broj: ",i+1);

            if(fgets(msg.tekst,128,stdin)==NULL){

                perror("Greska unos");
                exit(1);
            }

            msg.tekst[strcspn(msg.tekst,"\n")]='\0';

            if(msgsnd(redid,&msg,strlen(msg.tekst)+1,0)==-1){

                perror("Greska send");
                exit(1);
            }

        }

        wait(NULL);

        msgctl(redid,IPC_RMID,0);

    }


    return 0;
}
