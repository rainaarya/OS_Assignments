/* You need to write a system call, kernel_2d_memcpy(), which copies one 2D floating point matrix to another. You would require using kernel functions like
copy_from_user() and copy_to_user() to read data bytes from user space and write back to user space. */
SYSCALL_DEFINE4(kernel_2d_memcpy, float *, dst, float *, src, int, n_rows, int, n_cols)
{
  int i, j;
  for (i = 0; i < n_rows; i++)
  {
    for (j = 0; j < n_cols; j++)
    {
      float val;
      copy_from_user(&val, src, sizeof(float));
      copy_to_user(dst, &val, sizeof(float));
      src++;
      dst++;
    }
  }
  return 0;
}
