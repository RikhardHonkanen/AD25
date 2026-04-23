#include "rect.h"
#include <stdio.h>

int main(void)
{
    double area = 0;

    rect_id r1 = rect_create(10.5, 20.5);
    (void)rect_get_area(r1, &area);
    printf("Rect 1 Area: %g\n", area);

    rect_id r2 = rect_create(100.23, 200.45);
    (void)rect_get_area(r2, &area);
    printf("Rect 2 Area: %g\n", area);

    return 0;
}