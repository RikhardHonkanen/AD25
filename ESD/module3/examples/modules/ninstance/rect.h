/**
 * @file rect.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An N instance rect module.
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

#define RECT_MAX_NUM 4

typedef int rect_id;

rect_id rect_create(double width, double length);

bool rect_set_width(rect_id id, double width);

bool rect_set_length(rect_id id, double length);

bool rect_get_area(rect_id id, double *area);

#endif /* RECT_H */