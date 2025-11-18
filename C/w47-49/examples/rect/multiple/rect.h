/**
 * @file rect.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A multiple instance rect module.
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
#include <stdbool.h>

typedef struct rect rect_t; // A forward declared type

rect_t *rect_create(double width, double length);

bool rect_set_width(rect_t *rct, double width);

bool rect_set_length(rect_t *rct, double length);

bool rect_get_area(rect_t *rct, double *area);

void rect_destroy(rect_t **rct);

#endif /* RECT_H */
