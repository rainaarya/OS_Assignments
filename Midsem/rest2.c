#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sysexits.h>

char *str="Hello World";

int main(){
    pid_t pid;
    pid=fork();
    if(pid==0){
        printf("Child process\n");
        str="Bye";
        //printf("%s\n",str);
        exit(0);
    }
    else if(pid>0){
        waitpid(pid,NULL,0);
        printf("Parent process\n");
        //str="Hello";
        printf("%s\n",str);
        exit(0);
    }
    else{
        printf("Error\n");
        exit(0);
    }
}