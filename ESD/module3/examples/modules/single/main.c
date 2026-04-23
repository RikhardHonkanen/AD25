#include "rect.h"
#include <stdio.h>

int main(void)
{
    rect_init(10.5, 20.5);
    printf("Rect Area: %g\n", rect_get_area());

    rect_set_length(20);
    printf("Rect Area: %g\n", rect_get_area());

    return 0;
}