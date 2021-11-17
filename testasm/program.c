/*#include <stdint.h>
#include <stdio.h>

extern long asmm();

int main(){
    long val = asmm();
    printf("%ld\n", val);
    return 0;
}
*/

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

uint64_t rdtsc()
{
    unsigned int lo, hi;
    __asm__ __volatile__("rdtsc"
                         : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

void main()
{

    int c;
    uint64_t tsc;
    uint64_t tsc2;

    // generate random number using rdrand instruction using inline assembly
    asm volatile("rdrand %0"
                 : "=r"(c));
    // read CPU time stamp counter using inline assembly
    //asm volatile("rdtsc" : "=A" (tsc));
    printf("%d\n", c);
    tsc = rdtsc();
    tsc2 = rdtsc();
    printf("%" PRIu64 "\n", (tsc));  // time in seconds since cpu reset


}