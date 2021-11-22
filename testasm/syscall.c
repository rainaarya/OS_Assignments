/* You need to write a system call, kernel_2d_memcpy(), which copies one 2D floating point matrix to another. You would require using kernel functions like
copy_from_user() and copy_to_user() to read data bytes from user space and write back to user space.*/
SYSCALL_DEFINE4(kernel_2d_memcpy, float *, dst, float *, src, int, n_rows, int, n_cols)
{
  int i, j;
  for (i = 0; i < n_rows; i++)
  {
    for (j = 0; j < n_cols; j++)
    {
      float val;
      if (copy_from_user(&val, src, sizeof(float)))
      {
        printk("Error copying from user space\n");
        return -1;
      }
      if (copy_to_user(dst, &val, sizeof(float)))
      {
        printk("Error copying to user space\n");
        return -1;
      }
      src++;
      dst++;
    }
  }
  return 0;
}

/* void copy(float *dest, float *src, int rows, int cols)
{
  float tempMatrix[rows][cols];
  memcpy(tempMatrix, src, rows * cols * sizeof(float));
  memcpy(dest, tempMatrix, rows * cols * sizeof(float));
}

int main()
{

  float a[3][3] = {{1, 2, 3},
                   {4, 5, 6},
                   {7, 8, 9}};
  float b[3][3] = {{11, 22, 33},
                   {44, 55, 66},
                   {77, 88, 99}};

  copy((float *)b, (float *)a, 3, 3);
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      printf("%f ", b[i][j]);
    }
    printf("\n");
  }
} */

/* void copy(float *dest, float *src, int rows, int cols)
{
  int i, j;
  
  for (i = 0; i < rows; i++)
  {
    for (j = 0; j < cols; j++)
    {
      float val;
      memcpy(&val, src, sizeof(float));
      memcpy(dest, &val, sizeof(float));
      src++;
      dest++;
    }
  }
}

int main()
{

  float a[3][3] = {{1, 2, 3},
                   {4, 5, 6},
                   {7, 8, 9}};
  float b[3][3] = {{11, 22, 33},
                   {44, 55, 66},
                   {77, 88, 99}};

  copy((float *)b, (float *)a, 3, 3);
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      printf("%f ", b[i][j]);
    }
    printf("\n");
  }
}
 */