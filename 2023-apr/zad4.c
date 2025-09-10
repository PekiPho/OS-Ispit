#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>

void findLargest(const char* dirPath,int level,char *path,long *largest){

    if(level>5) return;

    DIR* dir=opendir(dirPath);
    struct dirent *dirp;
    char path1[1024];
    struct stat st;

    while((dirp=readdir(dir))!=NULL){
        if(strcmp(dirp->d_name,".")==0 || strcmp(dirp->d_name,"..")==0) continue;

        strcpy(path1,dirPath);
        strcat(path1,"/");
        strcat(path1,dirp->d_name);

        stat(path1,&st);


        if(S_ISREG(st.st_mode)){
            if(st.st_size> *largest){

                *largest=st.st_size;
                strncpy(path,path1,1024);
            }
        }
        else if(S_ISDIR(st.st_mode)){
            findLargest(path1,level+1,path,largest);
        }
    }

}


int main(int argc,char* argv[]){

    char largestPath[1024]="";
    long largest=-1;

    findLargest(argv[1],0,largestPath,&largest);


    printf("%s",largestPath);
}
