#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>

void sgterm_handler(int signo, siginfo_t *siginfo, void *ucontext)
{
    printf("Handled SIGINT, recieved value is %d\n", siginfo->si_int);
}

void signal_handler(int signum, siginfo_t *siginfo, void *ucontext)
{
    // if (signum != SIGINT) return;
    // if (siginfo->si_code != SI_QUEUE) return;
    int n = siginfo->si_value.sival_int;
    char buff[1024];
    snprintf(buff, sizeof(buff), "receiver: Got value %d\n", n);
    write(STDOUT_FILENO, buff, strlen(buff));
}

int main()
{
    int parentId;
    parentId = getpid();
    
    printf("Parent process id is %d\n", getpid());
    //signal(SIGINT,sgterm_handler);
    //pid_t pid = getpid();
    struct sigaction signal_action;

    //printf("receiver: PID is %d\n", pid);

    signal_action.sa_sigaction = signal_handler;
    sigemptyset(&signal_action.sa_mask);
    signal_action.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &signal_action, NULL);
    
    pid_t pid = fork();

    if (pid == 0)
    {
        printf("Child process id is %d\n", getpid());

        union sigval vals;
        vals.sival_int = 7;
        printf("sender: sending %d to PID %d\n", vals.sival_int, parentId);
        sigqueue(parentId, SIGINT, vals);
    }
    else if (pid > 0)
    {
        while (1)
        sleep(100);
    }

    return 0;
}