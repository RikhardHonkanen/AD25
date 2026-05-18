#ifndef UTILITY_H
#define UTILITY_H

#include <stddef.h>
#include <stdbool.h>

bool utility_string_read(char *str, size_t size);

bool utility_string_write(const char *str);

void utility_string_reverse(char *str);

#endif
