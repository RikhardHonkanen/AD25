#include <stdio.h>

int main(void)
{
    char command = 0;

#if 0
    (void)printf("Press A: ");
    (void)scanf(" %c", &command);

    while (command != 'A')
    {
        (void)printf("\nPress A: ");
        (void)scanf(" %c", &command);
    }
#endif

    do
    {
        (void)printf("\nPress A: ");
        (void)scanf(" %c", &command);
    } while (command != 'A');

    return 0;
}