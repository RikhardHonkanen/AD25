#ifndef CONSOLE_H
#define CONSOLE_H

#define CONSOLE_EOF -1
#define CONSOLE_BUFSIZE 8192

int console_read(void);
int console_write(int c);

#endif