#include <stdio.h>

int main(void)
{
    int i = 0;

    while (i < 100)
    {
        i++;

        if ((i == 50) || (i == 75))
        {
            continue;
        }

        (void)printf("%d\t", i);
    }

    i = 1;

    while (i <= 10)
    {
        int j = 1;

        while (j <= 10)
        {
            (void)printf("%d\t", i * j);
            j++;
        }

        (void)printf("\n");

        i++;
    }

    return 0;
}