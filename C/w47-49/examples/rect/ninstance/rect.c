#include "rect.h"

typedef struct
{
    double width;
    double length;
} rect_t;

static int number = 0;
static rect_t rects[RECT_MAX_NUM] = {0};

rect_id rect_create(double width, double length)
{
    rect_id id = -1;

    if (number < RECT_MAX_NUM)
    {
        rects[number].width = width;
        rects[number].length = length;

        id = number;
        number++;
    }

    return id;
}

bool rect_set_width(rect_id id, double width)
{
    bool status = false;

    if ((id > -1) && (id < number))
    {
        status = true;
        rects[id].width = width;
    }

    return status;
}

bool rect_set_length(rect_id id, double length)
{
    bool status = false;

    if ((id > -1) && (id < number))
    {
        status = true;
        rects[id].length = length;
    }

    return status;
}

bool rect_get_area(rect_id id, double *area)
{
    bool status = false;

    if ((area != NULL) && (id > -1) && (id < number))
    {
        status = true;
        *area = rects[id].length * rects[id].width;
    }

    return status;
}