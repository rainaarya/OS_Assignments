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
#include <time.h>

#define PROCESSOR_FREQUENCY 2994373000

pid_t pid_s1;
unsigned long start_tsc;
time_t current_time;

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
        unsigned long tsc;
        tsc = rdtsc();
        time_t time_in_sec = current_time + tsc / PROCESSOR_FREQUENCY - start_tsc / PROCESSOR_FREQUENCY; // 2994373000 is the frequency of the CPU
        
        //convert to local time
        struct tm *local_time = localtime(&time_in_sec);

        key_t key = ftok("shmfile", 65);        
        int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
        char *str = (char *)shmat(shmid, (void *)0, 0);

        strftime(str, 26, "%Y-%m-%d %H:%M:%S", local_time);

        /* unsigned long long time_seconds = tsc / 2994373000;
        
        // convert time_seconds to days, hours, minutes, seconds
        unsigned long long days = time_seconds / 86400;
        unsigned long long hours = (time_seconds % 86400) / 3600;
        unsigned long long minutes = (time_seconds % 3600) / 60;
        unsigned long long seconds = time_seconds % 60;
        sprintf(str, "%llu days, %llu hours, %llu minutes, %llu seconds", days, hours, minutes, seconds); */

        sigval.sival_ptr = str;

        sigqueue(pid_s1, SIGTERM, sigval);
        
        
        shmdt(str);
    }
}

int main(int argc, char **argv)
{
    // get current rdtsc value
    start_tsc = rdtsc();
    // get current date and time
    current_time = time(0);

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