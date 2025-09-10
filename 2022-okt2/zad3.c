#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/msg.h>

struct poruka{

    long type;
    char text[128];
};


int main(int argc,char* argv[]){

    int pid2,pid3;
    int msgid2,msgid3;

    msgid2=msgget(10001,IPC_CREAT | 0666);
    msgid3=msgget(10002,IPC_CREAT | 0666);

    pid2=fork();
    if(pid2==0){

        struct poruka msg;

        while(1){

            msgrcv(msgid2,&msg,sizeof(msg.text),0,0);

            if(msg.text[0]=='\0') break;

            printf("%s \n %d",msg.text,strlen(msg.text));
        }

        exit(0);
    }

    pid3=fork();
    if(pid3==0){

        struct poruka msg;

        while(1){
            msgrcv(msgid3,&msg,sizeof(msg.text),0,0);

            if(msg.text[0]=='\0') break;

            for(int i=0;i<strlen(msg.text);i++){

                msg.text[i]=toupper(msg.text[i]);
            }

            printf("%s",msg.text);

        }
        exit(0);
    }

    int cifra;
    struct poruka buff;

    char s[128];

    while(1){

        printf("Unesi cifru: ");
        scanf("%d",&cifra);

        if(cifra!=1 && cifra !=2) break;

        printf("Unesi string: ");
        fgets(s,128,stdin);
        s[strcspn(s,"\n")]='\0';

        strcpy(buff.text,s);

        if(cifra==1){

            msgsnd(msgid2,&buff,strlen(buff.text)+1,0);
        }
        if(cifra==2){

            msgsnd(msgid3,&buff,strlen(buff.text)+1,0);
        }

    }

    buff.text[0]='\0';
    msgsnd(msgid2,&buff,1,0);
    msgsnd(msgid2,&buff,1,0);


    wait(NULL);
    wait(NULL);

    msgctl(msgid2,IPC_RMID,0);
    msgctl(msgid3,IPC_RMID,0);

    return 0;

}
