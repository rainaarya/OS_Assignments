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

pid_t pid_s1;

void sig_handler(int signo)
{
    if (signo == SIGALRM)
    {
        union sigval sigval;
        int c;
        
        // generate random number using rdrand instruction using inline assembly
        asm volatile("rdrand %0"
                     : "=r"(c));
        sigval.sival_int = c;
        sigqueue(pid_s1, SIGTERM, sigval);
    }
}
int main(int argc, char **argv)
{
    pid_s1 = atoi(argv[1]);
    struct itimerval timer;

    signal(SIGALRM, sig_handler);

    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, NULL);

    while (1)
        ;
}