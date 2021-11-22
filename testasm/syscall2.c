/* You need to write a system call, kernel_2d_memcpy(), which copies one 2D floating point matrix to another. You would require using kernel functions like
copy_from_user() and copy_to_user() to read data bytes from user space and write back to user space.*/

SYSCALL_DEFINE4(kernel_2d_memcpy, float*, dst, float*, src, int, n_rows, int, n_cols)
{
    float tempMatrix[n_rows][n_cols];
    copy_from_user(tempMatrix, src, sizeof(float)*n_rows*n_cols);
    copy_to_user(dst, tempMatrix, sizeof(float)*n_rows*n_cols);
    return 0;
}