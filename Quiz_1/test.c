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

int main()
{
    unsigned long long a=1234567812345669; 
    

    printf("%llx\n",a);
    sleep(2);
}