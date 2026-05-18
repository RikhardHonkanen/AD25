#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>
#include <stdbool.h>

void *malloc(size_t size); // Fake malloc

void free(void *ptr); // Fake free

// Helper functions
void stdlib_init(void);

void stdlib_set_malloc_status(bool first, bool second);

bool stdlib_all_blocks_released(void);

#endif