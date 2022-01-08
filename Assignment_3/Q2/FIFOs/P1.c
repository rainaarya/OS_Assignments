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
#include <time.h>

struct packet
{
    int id;
    char str[10];
};

//P1.c
int main()
{
    char arr[50][10];
    struct packet p[50];
    int end = 0;

    struct packet buffer[5];
    int idbuffer;
    srand(time(NULL));
    for (int i = 0; i < 50; i++)
    {
        p[i].id = i;
        for (int j = 0; j < 9; j++)
        {
            arr[i][j] = (char)(rand() % 26 + 65);
            p[i].str[j] = arr[i][j];
        }
        arr[i][9] = '\0';
        p[i].str[9] = '\0';
    }

    // Create a named pipe
    mkfifo("fifofile", 0666);
    memcpy(buffer, p, sizeof(buffer)); // copy the first 5 array of strings to the buffer

    while (end != 1)
    {
        int fd;
        if ((fd = open("fifofile", O_WRONLY)) == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        if (write(fd, buffer, sizeof(buffer)) == -1)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
        //print the buffer
        for (int i = 0; i < 5; i++)
        {
            printf("String sent with ID: %d     Content: %s\n", buffer[i].id, buffer[i].str);
        }
        if (close(fd) == -1)
        {
            perror("close");
            exit(EXIT_FAILURE);
        }
        sleep(1);

        if ((fd = open("fifofile", O_RDONLY)) == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        if (read(fd, &idbuffer, sizeof(idbuffer)) == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (idbuffer == 49)
        {
            printf("Acknowledgement recieved. Highest ID recieved from P2 is %d\n\n", idbuffer);
            end = 1;
            if (close(fd) == -1)
            {
                perror("close");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            printf("Acknowledgement recieved. Highest ID recieved from P2 is %d\n\n", idbuffer);
            idbuffer++;
            memcpy(buffer, p + idbuffer, sizeof(buffer));
            if (close(fd) == -1)
            {
                perror("close");
                exit(EXIT_FAILURE);
            }
        }
    }
    return 0;
}