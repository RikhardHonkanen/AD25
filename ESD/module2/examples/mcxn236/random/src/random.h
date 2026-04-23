#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>
#include <stdbool.h>

bool random_init(void);

uint32_t random_get(void);

#endif