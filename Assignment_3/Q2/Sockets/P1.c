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
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/un.h>

// P1.c

struct packet
{
    int id;
    char str[10];
};

int main(void)
{
    // generate an array of 50 random strings of fixed length.
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

    memcpy(buffer, p, sizeof(buffer)); // copy the first 5 array of strings to the buffer

    int s, t, len;
    struct sockaddr_un remote;
    //char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    printf("Trying to connect...\n");

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, "socketpath");
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(s, (struct sockaddr *)&remote, len) == -1)
    {
        perror("connect");
        exit(1);
    }

    printf("Connected.\n");

    while (end != 1)
    {
        if (send(s, buffer, sizeof(buffer), 0) == -1)
        {
            perror("send");
            exit(1);
        }
        //print the buffer
        for (int i = 0; i < 5; i++)
        {
            printf("String sent with ID: %d     Content: %s\n", buffer[i].id, buffer[i].str);
        }
        sleep(1);

        if ((t = recv(s, &idbuffer, sizeof(idbuffer), 0)) > 0)
        {
            printf("Acknowledgement Recieved. Highest ID recieved from P2 is %d\n\n", idbuffer);
            if (idbuffer == 49)
            {
                end = 1;
            }
            else
            {
                idbuffer++;
                memcpy(buffer, p + idbuffer, sizeof(buffer));
            }
        }
        else
        {
            if (t < 0)
                perror("recv");
            else
                printf("Server closed connection\n");
            exit(1);
        }
    }

    close(s);

    return 0;
}
