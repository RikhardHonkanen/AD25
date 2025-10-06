#include <stdio.h>

static void copy(int n, int *restrict src, int *restrict dst)
{
    while (n > 0)
    {
        *dst = *src; // None of the objects modified through *dst is
                     // the same as any of the objects read through *src
        dst++;
        src++;
        n--;
    }
}

int main(void)
{
    int arr[10] = {0};
    copy(5, &arr[0], &arr[5]); // OK
    copy(5, &arr[0], &arr[1]); // Undefined behavior: arr[1] is accessed through both src and dst in copy

    return 0;
}