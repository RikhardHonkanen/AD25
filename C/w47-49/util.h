/**
 * @file util.h
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Some utility functions
 * @version 0.1
 * @date 2025-11-20
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>
#include <stdlib.h>

#define MAX_VALUE 65535

// Don't send in bigger values than max!!!
uint32_t get_greatest_common_divisor(uint16_t a, uint16_t b);
uint32_t get_least_common_multiple(uint16_t a, uint16_t b);
char *sort_string(char *s);
#endif