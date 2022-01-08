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


// P1.c

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
    int end = 0;

    key = ftok("progfile", 65);

    if((msgid = msgget(key, 0666 | IPC_CREAT)) == -1)
    {
        perror("msgget");
        exit(1);
    }
    message.mtype = 1;

    char arr[50][10];
    struct packet p[50];

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

    memcpy(message.p, p, sizeof(message.p));

    while (end != 1)
    {

        if(msgsnd(msgid, &message, sizeof(message), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }
        //print the message
        for (int i = 0; i < 5; i++)
        {
            printf("String sent with ID: %d     Content: %s\n", message.p[i].id, message.p[i].str);
        }
        sleep(1);

        if(msgrcv(msgid, &idbuffer, sizeof(idbuffer), 2, 0) == -1)
        {
            perror("msgrcv");
            exit(1);
        }
        if (idbuffer.highid == 49)
        {
            printf("Acknowledgement Recieved. Highest ID recieved from P2 is %d\n\n", idbuffer.highid);
            end = 1;
        }
        else
        {
            printf("Acknowledgement Recieved. Highest ID recieved from P2 is %d\n\n", idbuffer.highid);

            memcpy(message.p, p + idbuffer.highid + 1, sizeof(message.p));
        }
    }

    if(msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        exit(1);
    }

    return 0;
}
