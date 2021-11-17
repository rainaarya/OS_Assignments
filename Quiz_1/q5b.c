#include <stdio.h>

int main(void)
{
    int *a = (int *)(0x1000);
    *a = 2;

    int *b = NULL;
    b = a;
    printf("%p", b);
    printf("%p", b + 1);
    printf("%p", (char *)b + 1);
    printf("%p", (void *)b + 1);

    //output
    //0x1004
    //0x1001
    //0x1001
}