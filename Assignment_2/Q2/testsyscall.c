#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

#define SYS_kernel_2d_memcpy 448 // number of syscall in kernel

int main()
{

  float matrix1[5][5] = {{1.23, 2.34, 3.45, 4.56, 5.67},
                         {6.78, 7.89, 8.90, 9.01, 10.12},
                         {11.23, 12.34, 13.45, 14.56, 15.67},
                         {16.78, 17.89, 18.90, 19.01, 20.12},
                         {21.23, 22.34, 23.45, 24.56, 25.67}};

  float matrix2[5][5] = {{3.21, 4.32, 5.43, 6.54, 7.65},
                         {8.76, 9.87, 10.98, 11.09, 12.20},
                         {13.31, 14.42, 15.53, 16.64, 17.75},
                         {18.86, 19.97, 21.08, 22.19, 23.30},
                         {24.41, 25.52, 26.63, 27.74, 28.85}};

  int rows = 5, cols = 5;

  printf("\nInitially, matrix 2 is:\n");
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      printf("%.2f ", matrix2[i][j]);
    }
    printf("\n");
  }

  
  int ret = syscall(SYS_kernel_2d_memcpy, matrix2, matrix1, rows, cols);
  printf("\nSystem call returned %d.\n", ret);

  if (ret == 0)
  {
    printf("\nMatrix 1 Copied to Matrix 2!\n\nNow Matrix 2 is:\n");
    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
      {
        printf("%.2f ", matrix2[i][j]);
      }
      printf("\n");
    }
  }
  else
  {
    printf("Error occured during system call.\n");
  }

  return 0;
}