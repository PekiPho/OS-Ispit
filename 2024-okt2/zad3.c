#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/msg.h>

struct poruka{

    long type;
    int src;
    char text[256];
};


int main(int argc,char* argv[]){

    int pid2,pid3;
    int msgid2,msgid3;

    msgid2=msgget(10001,IPC_CREAT | 0666);

    pid2=fork();
    if(pid2==0){

        FILE *f=fopen("prva.txt","r");

        char buff[256];
        struct poruka msg;
        msg.type=1;
        msg.src=1;

        while(fgets(buff,256,f)!=NULL){
            strcpy(msg.text,buff);

            msgsnd(msgid2,&msg,sizeof(msg) - sizeof(long),0);

        }

        msg.text[0]='\0';
        msgsnd(msgid2,&msg,sizeof(msg)-sizeof(long),0);

        fclose(f);

        exit(0);
    }

    pid3=fork();
    if(pid3==0){

        FILE* f=fopen("druga.txt","r");

        char buff[256];
        struct poruka msg;
        msg.type=1;
        msg.src=2;

        while(fgets(buff,256,f)!=NULL){
            strcpy(msg.text,buff);

            msgsnd(msgid2,&msg,sizeof(msg) - sizeof(long),0);
        }

        msg.text[0]='\0';
        msgsnd(msgid2,&msg,sizeof(msg)-sizeof(long),0);

        fclose(f);

        exit(0);
    }


    FILE* f1=fopen("prva-mod.txt","w");
    FILE* f2=fopen("druga-mod.txt","w");

    struct poruka msg;

    int finished=0;
    while(finished<2){

        msgrcv(msgid2,&msg,sizeof(msg) - sizeof(long),0,0);

        if(msg.text[0]=='\0'){
            finished++;
            continue;
        }


        if(msg.src==1){

            for(int i=0;i<strlen(msg.text);i++){
                msg.text[i]=toupper(msg.text[i]);
            }
            fputs(msg.text,f1);
        }
        if(msg.src==2){

            for(int i=0;i<strlen(msg.text);i++){
                msg.text[i]=tolower(msg.text[i]);
            }
            fputs(msg.text,f2);
        }
    }


    wait(NULL);
    wait(NULL);

    msgctl(msgid2,IPC_RMID,0);

    return 0;
}
