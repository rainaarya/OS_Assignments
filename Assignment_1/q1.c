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

extern int errno;

void getInfo(char *section[26], int studentId[26], int marks[6][26])
{
    int fd = open("student_record.csv", O_RDONLY | O_EXCL);
    if (fd == -1)
    {
        perror("open");
        return;
    }
    char *c = (char *)calloc(1000, sizeof(char));
    char *data[27];
    int sz = read(fd, c, 1000);
    if (sz < 0)
    {
        perror("read");
        return;
    }
    char *token = strtok(c, "\n");

    int itr = 0;
    while (token != NULL)
    {
        data[itr] = token;
        token = strtok(NULL, "\n");
        itr++;
    }
    for (int i = 0; i < 26; i++)
    {
        token = strtok(data[i + 1], ",");
        studentId[i] = atoi(token);
        token = strtok(NULL, ",");
        section[i] = token;
        token = strtok(NULL, ",");
        marks[0][i] = atoi(token);
        token = strtok(NULL, ",");
        marks[1][i] = atoi(token);
        token = strtok(NULL, ",");
        marks[2][i] = atoi(token);
        token = strtok(NULL, ",");
        marks[3][i] = atoi(token);
        token = strtok(NULL, ",");
        marks[4][i] = atoi(token);
        token = strtok(NULL, ",");
        marks[5][i] = atoi(token);
    }
    if (close(fd) < 0)
    {
        perror("close");
        return;
    }
}

void printToScreen(char *str)
{
    size_t w = write(1, str, strlen(str));
    if (w < 0)
    {
        if (errno != EINTR)
        {
            perror("write");
            return;
        }
    }
}

int main()
{
    int studentId[26];
    char *section[26];
    int marks[6][26];
    int stat;
    double avg = 0;
    char str[1000000];
    pid_t id;
    id = fork();
    if (id == 0)
    {
        getInfo(section, studentId, marks);

        for (int j = 0; j < 6; ++j)
        {
            avg = 0;
            int count = 0;
            for (int i = 0; i < 26; i++)
            {
                if (strcmp(section[i], "A") == 0)
                {
                    avg += marks[j][i];
                    ++count;
                }
            }
            avg /= count;
            snprintf(str, 10000, "%d) The average marks of all students in section A in Assignment %d is %lf\n", j + 1, j + 1, avg);
            printToScreen(str);
        }
        printf("\n");
        exit(0);
    }
    else if (id > 0)
    {
        while (waitpid(id, &stat, 0) == -1)
        {
            if (stat != EINTR)
            {
                perror("waitpid");
                return EX_SOFTWARE;
            }
        }
        getInfo(section, studentId, marks);
        for (int j = 0; j < 6; ++j)
        {
            avg = 0;
            int count = 0;
            for (int i = 0; i < 26; i++)
            {
                if (strcmp(section[i], "B") == 0)
                {
                    avg += marks[j][i];
                    ++count;
                }
            }
            avg /= count;
            snprintf(str, 10000, "%d) The average marks of all students in section B in Assignment %d is %lf\n", j + 1, j + 1, avg);
            printToScreen(str);
        }
    }
    else
    {
        perror("fork");
        return EX_OSERR;
    }
    return 0;
}