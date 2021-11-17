#include <stdint.h>
#include <stdio.h>

extern void B(long);

void A()
{
    long long num;
    printf("Currently Inside Function A\n");
    printf("Enter input number: ");
    scanf("%lld", &num);
    printf("\n");
    //B(491495383406);
    //B(418648252782);
    B(num);
}

int main()
{
    A();

    return 0;
}