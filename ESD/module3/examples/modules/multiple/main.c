#include "rect.h"
#include <stdio.h>

int main(void)
{
    double area = 0;

    rect_t *r1 = rect_create(10.5, 20.5);
    if (r1 == NULL)
    {
        printf("Failed to create an instance\n");
        exit(1);
    }
    (void)rect_get_area(r1, &area);
    printf("Rect 1 Area: %g\n", area);

    rect_t *r2 = rect_create(100.23, 200.45);
    if (r2 == NULL)
    {
        rect_destroy(&r1);
        printf("Failed to create an instance\n");
        exit(1);
    }
    (void)rect_get_area(r2, &area);
    printf("Rect 2 Area: %g\n", area);

    rect_destroy(&r1);
    rect_destroy(&r2);

    return 0;
}