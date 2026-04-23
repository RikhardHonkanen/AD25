#include <stdio.h>
#include "utility.h"

int main(void)
{
    utility_console_t output = {.eof = EOF, .write = putchar};

    utility_init(&output);

    (void)utility_write_int(1234);

    (void)printf("\n");

    return 0;
}
