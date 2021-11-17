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
#include <sys/ipc.h>
#include <sys/shm.h>

pid_t pid_s1;

uint64_t rdtsc()
{
    unsigned int lo, hi;
    __asm__ __volatile__("rdtsc"
                         : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

void sig_handler(int signo)
{
    if (signo == SIGALRM)
    {
        union sigval sigval;

        // read CPU time stamp counter using inline assembly
        unsigned long long tsc;
        tsc = rdtsc();
        //sigval.sival_int = tsc;

        key_t key = ftok("shmfile", 65);

        // shmget returns an identifier in shmid
        int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

        // shmat to attach to shared memory
        char *str = (char *)shmat(shmid, (void *)0, 0);

        
        
        unsigned long long time_seconds = tsc / 2994373000;
        
        // convert time_seconds to days, hours, minutes, seconds
        unsigned long long days = time_seconds / 86400;
        unsigned long long hours = (time_seconds % 86400) / 3600;
        unsigned long long minutes = (time_seconds % 3600) / 60;
        unsigned long long seconds = time_seconds % 60;
        sprintf(str, "%llu days, %llu hours, %llu minutes, %llu seconds", days, hours, minutes, seconds);
        sigval.sival_ptr = str;

        

        sigqueue(pid_s1, SIGTERM, sigval);
        //detach from shared memory
        shmdt(str);

    }
}

int main(int argc, char **argv)
{
    pid_s1 = atoi(argv[1]);
    struct itimerval timer;

    /* Install timer_handler as the signal handler for SIGVTALRM. */
    signal(SIGALRM, sig_handler);

    /* Configure the timer to expire after 1 sec... */
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    /* ... and every 1 sec after that. */
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    /* Start a real timer. It counts down whenever this process is
   executing. */
    setitimer(ITIMER_REAL, &timer, NULL);

    /* Do busy work. */
    while (1)
        ;
}