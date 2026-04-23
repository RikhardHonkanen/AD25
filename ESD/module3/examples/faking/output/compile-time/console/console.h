#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdio.h>

#define CONSOLE_EOF EOF

static inline int console_write(int c) { return putchar(c); }

#endif