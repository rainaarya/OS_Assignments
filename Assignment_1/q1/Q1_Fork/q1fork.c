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
        exit(1);
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
        exit(1);
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
        exit(1);
    }
}

int main()
{
    getBufferSize();
    getStudentNumber();

    int stuID[total_students];
    char *section[total_students];
    int assgn_marks[6][total_students];
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
            for (int i = 0; i < total_students; i++)
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
        
        exit(0);    // child calls exit() system call
    }
    else if (id > 0)
    {
        

        if (waitpid(id, &stat, 0) == -1)
        {
            perror("waitpid");
            return 1;
        }
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