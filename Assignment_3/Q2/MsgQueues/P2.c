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

// P2.c

// Defining the structure of the message
struct msg_st
{
    long mtype;
    struct packet
    {
        int id;
        char str[10];
    } p[5];
} message;

struct msg_highid
{
    long mtype;
    int highid;
} idbuffer;

int main()
{
    key_t key;
    int msgid;

    key = ftok("progfile", 65);

    if((msgid = msgget(key, 0666 | IPC_CREAT)) == -1)
    {
        perror("msgget");
        exit(1);
    }
    idbuffer.mtype = 2;

    while (idbuffer.highid != 49)
    {
        if(msgrcv(msgid, &message, sizeof(message), 1, 0) == -1)
        {
            perror("msgrcv");
            exit(1);
        }
        int highid = message.p[0].id;
        for (int i = 0; i < 5; i++)
        {
            printf("String recieved with ID: %d     Content: %s\n", message.p[i].id, message.p[i].str);

            if (message.p[i].id > highid)
            {
                highid = message.p[i].id;
            }
        }
        printf("\n");
        idbuffer.highid = highid;

        if(msgsnd(msgid, &idbuffer, sizeof(idbuffer), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }
    }


    return 0;
}
