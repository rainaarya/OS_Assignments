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

void getInfo(int stuID[26], char *section[26], int assgn_marks[6][26])
{
    int fd = open("student_record.csv", O_RDONLY | O_EXCL);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
    char *c = (char *)calloc(1000, sizeof(char));
    char *data[27];
    if (read(fd, c, 1000) < 0)
    {
        perror("read");
        exit(1);
    }
    char *token = strtok(c, "\n");

    int line = 0;
    while (token != NULL)
    {
        data[line] = token;
        token = strtok(NULL, "\n");
        line++;
    }
    for (int i = 0; i < 26; i++)
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
        exit(1);
    }
}

int main()
{
    int stuID[26];
    char *section[26];
    int assgn_marks[6][26];
    int stat;
    double avg = 0;

    pid_t id;
    id = fork();
    if (id == 0)
    {
        getInfo(stuID, section, assgn_marks);

        for (int j = 0; j < 6; ++j)
        {
            avg = 0;
            int count = 0;
            for (int i = 0; i < 26; i++)
            {
                if (strcmp(section[i], "A") == 0)
                {
                    avg += assgn_marks[j][i];
                    ++count;
                }
            }
            avg /= count;
            printf("%d) The average marks of all students in section A in Assignment %d is %lf\n", j + 1, j + 1, avg);
        }
        printf("\n");
        exit(0);
    }
    else if (id > 0)
    {
        /* while (waitpid(id, &stat, 0) == -1)
        {
            if (stat != EINTR)
            {
                perror("waitpid");
                return EX_SOFTWARE;
            }
        } */

        if (waitpid(id, &stat, 0) == -1)
        {
            if (stat != EINTR)
            {
                perror("waitpid");
                return 1;
            }
        }
        getInfo(stuID, section, assgn_marks);
        for (int j = 0; j < 6; ++j)
        {
            avg = 0;
            int count = 0;
            for (int i = 0; i < 26; i++)
            {
                if (strcmp(section[i], "B") == 0)
                {
                    avg += assgn_marks[j][i];
                    ++count;
                }
            }
            avg /= count;
            printf("%d) The average marks of all students in section B in Assignment %d is %lf\n", j + 1, j + 1, avg);
        }
    }
    else
    {
        perror("fork");
        return 1;
    }
    return 0;
}