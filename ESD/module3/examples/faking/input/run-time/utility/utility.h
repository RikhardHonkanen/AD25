#ifndef utility_H
#define utility_H

#include <stdbool.h>

typedef int (*utility_getchar_t)(void);

void utility_init(utility_getchar_t fptr);

bool utility_read_int(int *number);

#endif
