#include <stdio.h>
int main()
{
    int x = -2;
    unsigned int y = -33;
    int z;
    z = x + y;
    printf("%u %u %u", x, y, z);
    printf(" %d %d %d", x, y, z);

    return 0;
}