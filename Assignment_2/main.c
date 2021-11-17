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

/* pid_t child_st_pid;
pid_t child_sr_pid; */

int *child_st_pid;
int *child_sr_pid;

void sig_handler(int signum, siginfo_t *siginfo, void *extra)
{
    if (siginfo->si_code != SI_QUEUE)
        return;

    pid_t sender_pid = siginfo->si_pid;

    if (sender_pid == *child_sr_pid)
    {
        int num = siginfo->si_value.sival_int;
        char buffer[1000];
        snprintf(buffer, sizeof(buffer), "SIGTERM Received and Random Number is %d\n", num);
        write(STDOUT_FILENO, buffer, strlen(buffer));
        
    }
    else if (sender_pid == *child_st_pid)
    {
        /* int num = siginfo->si_value.sival_int;
        char buffer[1000];
        snprintf(buffer, sizeof(buffer), "SIGTERM Received and CPU Clock Cycle Time is %d\n", num);
        write(STDOUT_FILENO, buffer, strlen(buffer));
        fflush(stdout); */

        // ftok to generate unique key
        key_t key = ftok("shmfile", 65);

        // shmget returns an identifier in shmid
        int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

        // shmat to attach to shared memory
        char *str = (char *)shmat(shmid, (void *)0, 0);

        char buffer[1000];
        snprintf(buffer, sizeof(buffer), "SIGTERM Received and CPU Clock Cycle Time is: ");
        write(STDOUT_FILENO, buffer, strlen(buffer));
        write(STDOUT_FILENO, str, strlen(str));
        // write \n
        write(STDOUT_FILENO, "\n", 1);
        


        //detach from shared memory
        shmdt(str);
    }
    else
    {
        char buffer[1000];
        snprintf(buffer, sizeof(buffer), "SIGTERM Received from process %d\n", sender_pid);
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }
}
int main()
{
    pid_t pid;
    child_st_pid = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    child_sr_pid = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    
    pid = fork();
    if (pid == 0)
    {
        struct sigaction signal_action;
        signal_action.sa_sigaction = sig_handler;
        sigemptyset(&signal_action.sa_mask);
        signal_action.sa_flags = SA_SIGINFO;
        sigaction(SIGTERM, &signal_action, NULL);

        while (1)
            ; // Infinite loop for S1 child process
    }
    else if (pid > 0)
    {
        pid_t pid_s1 = pid; // pid of s1 child

        // now create two child processes from parent
        pid_t st, sr;

        st = fork();

        if (st == 0)
        {
            /* Child A code */
            *child_st_pid = getpid();
            //printf("Child ST PID is %d\n", child_st_pid);
            char pid_char[50];
            snprintf(pid_char, sizeof(pid_char), "%d", pid_s1);
            char *args[] = {"./ST", pid_char, NULL};
            execv(args[0], args);
        }
        else
        {
            sr = fork();

            if (sr == 0)
            {
                /* Child B code */
                *child_sr_pid = getpid();
                //printf("Child SR PID is %d\n", child_sr_pid);
                char pid_char[50];
                snprintf(pid_char, sizeof(pid_char), "%d", pid_s1);
                char *args[] = {"./SR", pid_char, NULL};
                execv(args[0], args);
            }
            else
            {
                /* Parent Code */
            }
        }
    }
    else
    {
        perror("fork");
    }
}