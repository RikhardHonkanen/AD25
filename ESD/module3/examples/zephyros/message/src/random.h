#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>
#include <cstddef>

bool random_init(void);

void random_get(uint8_t *data, size_t size);

#endif