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
#include <pthread.h>
#include <semaphore.h>

sem_t forks[5];
sem_t sauce_bowls;

void *philosopher(void *arg)
{

    int n = *((int *)arg);
    int left = n;
    int right = (n + 1) % 5;
    while (1)
    {

        printf("Philosopher %d is thinking\n", n);
        sleep(1);
        printf("Philosopher %d is hungry\n", n);

        //If there is atleast one philosipher who takes fork first with right hand, then it breaks the circle to avoid deadlock.
        if (n == 4)
        {
            sem_wait(&forks[right]);
            printf("Philosopher %d has picked up right fork with fork no %d\n", n, right);
            sem_wait(&forks[left]);
            printf("Philosopher %d has picked up left fork with fork no %d\n", n, left);
            sem_wait(&sauce_bowls);
            printf("Philosopher %d has picked up a sauce bowl\n", n);
        }
        else
        {
            sem_wait(&forks[left]);
            printf("Philosopher %d has picked up left fork with fork no %d\n", n, left);
            sem_wait(&forks[right]);
            printf("Philosopher %d has picked up right fork with fork no %d\n", n, right);
            sem_wait(&sauce_bowls);
            printf("Philosopher %d has picked up a sauce bowl\n", n);
        }
        printf("Philosopher %d is eating\n", n);
        sleep(2);
        printf("Philosopher %d is done eating\n", n);
        //drop the forks and sauce_bowls
        sem_post(&sauce_bowls);
        printf("Philosopher %d has dropped a sauce bowl\n", n);
        sem_post(&forks[left]);
        printf("Philosopher %d has dropped left fork with fork no %d\n", n, left);
        sem_post(&forks[right]);
        printf("Philosopher %d has dropped right fork with fork no %d\n", n, right);
    }
}

int main(int argc, char *argv[])
{

    pthread_t philosophers[5];
    sem_init(&sauce_bowls, 0, 4); //initialize sauce_bowls to 4

    for (int i = 0; i < 5; i++)
    {
        sem_init(&forks[i], 0, 1);
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&philosophers[i], NULL, philosopher, (void *)arg);
    }
    for (int i = 0; i < 5; i++)
    {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}
