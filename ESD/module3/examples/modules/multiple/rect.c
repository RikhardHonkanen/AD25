#include "rect.h"

struct rect
{
    double width;
    double length;
};

rect_t *rect_create(double width, double length)
{
    rect_t *rct = (rect_t *)malloc(sizeof(rect_t));

    if (rct != NULL)
    {
        rct->width = width;
        rct->length = length;
    }

    return rct;
}

bool rect_set_width(rect_t *rct, double width)
{
    bool status = false;

    if (rct != NULL)
    {
        rct->width = width;
    }

    return status;
}

bool rect_set_length(rect_t *rct, double length)
{
    bool status = false;

    if (rct != NULL)
    {
        rct->length = length;
    }

    return status;
}

bool rect_get_area(rect_t *rct, double *area)
{
    bool status = false;

    if ((rct != NULL) && (area != NULL))
    {
        *area = rct->length * rct->width;
    }

    return status;
}

void rect_destroy(rect_t **rct)
{
    if ((rct != NULL) && (*rct != NULL))
    {
        free(*rct);
        *rct = NULL;
    }
}
