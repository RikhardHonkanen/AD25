#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdio.h>

#define CONSOLE_EOF EOF
#define CONSOLE_BUFSIZE BUFSIZE

static inline int console_read(void) { return getchar(); }
static inline int console_write(int c) { return putchar(c); }

#endif