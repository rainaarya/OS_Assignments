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
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// P2.c

struct packet
{
    int id;
    char str[10];
};

int main()
{
    struct packet buffer[5];
    mkfifo("fifofile", 0666);
    int id, fd;

    while (id != 49)
    {
        if((fd = open("fifofile", O_RDONLY)) == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        if(read(fd, &buffer, sizeof(buffer)) == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        int highest = buffer[0].id;

        for (int i = 0; i < 5; i++)
        {
            printf("String recieved with ID: %d     Content: %s\n", buffer[i].id, buffer[i].str);
            if (buffer[i].id > highest)
            {
                highest = buffer[i].id;
            }
        }
        printf("\n");
        id = highest;
        close(fd);

        if((fd = open("fifofile", O_WRONLY)) == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        if(write(fd, &id, sizeof(id)) == -1)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
        if(close(fd) == -1)
        {
            perror("close");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
