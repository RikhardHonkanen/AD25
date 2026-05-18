#ifndef UTILITY_H
#define UTILITY_H

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    int eof;
    int (*read)(void);
    int (*write)(int);
} utility_console_t;

void utility_init(const utility_console_t *ptr);

bool utility_string_read(char *str, size_t size);

bool utility_string_write(const char *str);

void utility_string_reverse(char *str);

#endif
