#include <stdio.h>
#include "console.h"

int console_read(void)
{
    return getchar();
}

int console_write(int c)
{
    return putchar(c);
}