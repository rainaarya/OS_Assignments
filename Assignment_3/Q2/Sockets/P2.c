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

// P2.c

struct packet
{
    int id;
    char str[10];
};

int main(void)
{
    int s, s2, len, id;
    unsigned t;
    struct sockaddr_un local, remote;
    struct packet buffer[5];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, "socketpath");
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1)
    {
        perror("bind");
        exit(1);
    }

    if (listen(s, 5) == -1)
    {
        perror("listen");
        exit(1);
    }

    while (id != 49)
    {
        int done, n;
        printf("Waiting for a connection...\n");
        t = sizeof(remote);
        if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1)
        {
            perror("accept");
            exit(1);
        }

        printf("Connected.\n");

        done = 0;
        do
        {
            n = recv(s2, buffer, sizeof(buffer), 0);

            if (n <= 0)
            {
                if (n < 0)
                    perror("recv");
                done = 1;
            }

            if (!done)
            {
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

                if (send(s2, &id, sizeof(id), 0) < 0)
                {
                    perror("send");
                    done = 1;
                }
            }
        } while (!done);

        close(s2);
    }

    return 0;
}
