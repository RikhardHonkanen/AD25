#include "console.h"
#include <stdio.h>
#include <string.h>

static size_t indx = 0;
static char buffer[BUFSIZ] = {0};

int console_read(void)
{
    char chr = buffer[indx];
    buffer[indx] = 0;
    indx = (indx + 1) % BUFSIZ;
    return chr;
}

void console_set_input(const char *str)
{
    indx = 0;
    memset(buffer, 0, sizeof(buffer));

    int len = strlen(str);
    strncpy(buffer, str, (len > BUFSIZ) ? BUFSIZ : len);
}