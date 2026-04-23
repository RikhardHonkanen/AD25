#include "rect.h"

typedef struct
{
    double width;
    double length;
} rect_t;

static rect_t rect = {0, 0};

void rect_init(double _width, double _length)
{
    rect.width = _width;
    rect.length = _length;
}

void rect_set_width(double _width)
{
    rect.width = _width;
}

void rect_set_length(double _length)
{
    rect.length = _length;
}

double rect_get_area(void)
{
    return (rect.width * rect.length);
}