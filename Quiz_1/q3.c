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

 int main()
{
    pid_t pid1;
    printf("beforefork()");
    if ((pid1 = fork()) > 0)
    {
        waitpid(pid1, NULL, 0);
    }
    else if (pid1 == 0)
    {
        execl("/usr/bin/bash", "bash", NULL);
        printf("done launching the shell");
    }

    else
    {
        perror("fork()");
    }
}

/* int main()
{
    printf("hello");
    sleep(2);
    pid_t id = fork();
    sleep(2);
    if (id == 0)
    {
        sleep(2);
        printf("Bye");
    }
    //return 0;
} */