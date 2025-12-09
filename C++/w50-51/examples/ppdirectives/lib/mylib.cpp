#include "mylib.h"
#include <cstdio>

#ifndef MYLIB_BUFSIZE
#error "MYLIB_BUFSIZE has not been defined"
#else
#if (MYLIB_BUFSIZE % 8 != 0)
#error "The value of MYLIB_BUFSIZE shall be a multiple of 8"
#endif
#endif

void print(void)
{
    (void)printf("MYLIB_BUFSIZE = %d\n", MYLIB_BUFSIZE);
}