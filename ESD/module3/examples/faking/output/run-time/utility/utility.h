#ifndef UTILITY_H
#define UTILITY_H

#include <stdbool.h>

typedef struct
{
    int eof;
    int (*write)(int);
} utility_console_t;

void utility_init(const utility_console_t *ptr);

bool utility_write_int(int number);

#endif
