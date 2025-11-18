#include "rect.h"

static double width = 0;
static double length = 0;

void rect_init(double _width, double _length)
{
    width = _width;
    length = _length;
}

void rect_set_width(double _width)
{
    width = _width;
}

void rect_set_length(double _length)
{
    length = _length;
}

double rect_get_area(void)
{
    return (width * length);
}