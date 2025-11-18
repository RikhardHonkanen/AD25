/**
 * @file rect.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An single instance rect module.
 *
 * @version 0.1
 * @date 2023-06-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef RECT_H
#define RECT_H

#include <stdlib.h>

void rect_init(double _width, double _length);

void rect_set_width(double _width);

void rect_set_length(double _length);

double rect_get_area(void);

#endif /* RECT_H */