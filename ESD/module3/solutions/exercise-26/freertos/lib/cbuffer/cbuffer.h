#ifndef CBUFFER_H
#define CBUFFER_H

#include <stdbool.h>

#define CBUFFER_SIZE 8

bool cbuffer_init(void);

void cbuffer_store(int value);

int cbuffer_restore(void);

void cbuffer_deinit(void);

#endif