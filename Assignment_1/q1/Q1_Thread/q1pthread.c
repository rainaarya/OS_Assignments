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

double avg_all[6];
int count_all[6];
int total_students;
int buffer_size;

void getBufferSize()
{
    struct stat st;
    if (stat("student_record.csv", &st) == -1)
    {
        perror("stat");
        exit(1);
    }
    int size;
    size = st.st_size;
    buffer_size = size;
}

void getStudentNumber()
{
    int fd = open("student_record.csv", O_RDONLY | O_EXCL);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }

    char *buffer = (char *)calloc(buffer_size, sizeof(char));
    if (buffer == NULL)
    {
        perror("calloc");
        exit(1);
    }

    if (read(fd, buffer, buffer_size) < 0)
    {
        perror("read");
        exit(1);
    }
    char *token = strtok(buffer, "\n");

    int line = 0;
    while (token != NULL)
    {
        token = strtok(NULL, "\n");
        line++;
    }

    total_students = line - 1;
}

void getInfo(int stuID[], char *section[], int assgn_marks[][total_students])
{
    int fd = open("student_record.csv", O_RDONLY | O_EXCL);
    if (fd == -1)
    {
        perror("open");
        return;
    }
    char *buffer = (char *)calloc(buffer_size, sizeof(char));
    if (buffer == NULL)
    {
        perror("calloc");
        exit(1);
    }
    char *data[total_students + 1];
    if (read(fd, buffer, buffer_size) < 0)
    {
        perror("read");
        return;
    }
    char *token = strtok(buffer, "\n");

    int line = 0;
    while (token != NULL)
    {
        data[line] = token;
        token = strtok(NULL, "\n");
        line++;
    }
    for (int i = 0; i < total_students; i++)
    {
        token = strtok(data[i + 1], ",");
        stuID[i] = atoi(token); // ID
        token = strtok(NULL, ",");
        section[i] = token; // Section
        token = strtok(NULL, ",");
        assgn_marks[0][i] = atoi(token); //Assignment 1
        token = strtok(NULL, ",");
        assgn_marks[1][i] = atoi(token); //Assignment 2
        token = strtok(NULL, ",");
        assgn_marks[2][i] = atoi(token); //Assignment 3
        token = strtok(NULL, ",");
        assgn_marks[3][i] = atoi(token); //Assignment 4
        token = strtok(NULL, ",");
        assgn_marks[4][i] = atoi(token); //Assignment 5
        token = strtok(NULL, ",");
        assgn_marks[5][i] = atoi(token); //Assignment 6
    }
    if (close(fd) == -1)
    {
        perror("close");
        return;
    }
}

void *parent(void *arg)
{

    int stuID[total_students];
    char *section[total_students];
    int assgn_marks[6][total_students];
    double avg = 0;

    getInfo(stuID, section, assgn_marks);
    for (int j = 0; j < 6; ++j)
    {
        avg = 0;
        int count = 0;
        for (int i = 0; i < total_students; i++)
        {
            if (strcmp(section[i], "B") == 0)
            {
                avg += assgn_marks[j][i];
                avg_all[j] += assgn_marks[j][i];
                ++count_all[j];
                ++count;
            }
        }
        avg /= count;
        printf("%d) The average marks of all students in section B in Assignment %d is %lf\n", j + 1, j + 1, avg);
    }

    printf("\n");
}

void *child(void *arg)
{

    int stuID[total_students];
    char *section[total_students];
    int assgn_marks[6][total_students];
    double avg = 0;
    getInfo(stuID, section, assgn_marks);

    for (int j = 0; j < 6; ++j)
    {
        avg = 0;
        int count = 0;
        for (int i = 0; i < total_students; i++)
        {
            if (strcmp(section[i], "A") == 0)
            {
                avg += assgn_marks[j][i];
                avg_all[j] += assgn_marks[j][i];
                ++count_all[j];
                ++count;
            }
        }
        avg /= count;
        printf("%d) The average marks of all students in section A in Assignment %d is %lf\n", j + 1, j + 1, avg);
    }
    printf("\n");
}

int main()
{
    getBufferSize();
    getStudentNumber();

    pthread_t newthread1, newthread2;

    if (pthread_create(&newthread1, NULL, child, NULL) != 0)
    {
        perror("pthread_create");
        return 1;
    }
    if (pthread_join(newthread1, NULL) != 0) //wait for child thread
    {
        perror("pthread_join");
        return 1;
    }

    if (pthread_create(&newthread2, NULL, parent, NULL) != 0)
    {
        perror("pthread_create");
        return 1;
    }
    if (pthread_join(newthread2, NULL) != 0)
    {
        perror("pthread_join");
        return 1;
    }

    for (int i = 0; i < 6; ++i)
    {
        printf("Average marks of all students across both sections in Assignment %d is %lf\n", i + 1, avg_all[i] / count_all[i]);
    }

    return 0;
}