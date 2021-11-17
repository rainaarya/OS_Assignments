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

 void copy_arr(char *p1, char p2[])
{
    memcpy(p2, p1, sizeof(p1));
    memcpy(p2, "ABCD", 4);
}
int main()
{
    char arr1[100];
    char arr2[100];
    
    
    printf("Enter a string:");
    scanf("%[^\n]s", arr1);
    copy_arr(arr1, arr2);
    printf("\n%s\n", arr2);
    
    return 0;
}

/* A C program to demonstrate working of memcpy */
/*
#include <stdio.h>
#include <string.h>
 
int main ()
{
  char str1[] = "geekslol";
  char str2[] = "quiz";
 
  puts("str1 before memcpy ");
  printf("%s\n",str1);
 
  // Copies contents of str2 to str1
  memcpy (str1, str2, 5);
 
  puts("\nstr1 after memcpy ");
  printf("%s\n",str1);
  //printf("%d",sizeof("HELLO"));
 
  return 0;
  0101011001111000
  0000000000010001
  0101011001101001

}*/