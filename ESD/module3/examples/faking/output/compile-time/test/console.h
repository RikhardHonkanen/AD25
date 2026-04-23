#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdbool.h>

#define CONSOLE_EOF -1
#define CONSOLE_BUFSIZE 8192

int console_write(int c);

// Helper functions
void console_set_status(bool _status);
const char *console_get_output(void);

#endif