#include <stdio.h>

int main(void)
{
    int a = 10;

loop:
    printf("%d\n", a);
    a--;
    if (a > 0)
    {
        goto loop;
    }

    goto exit;

    a = 2 * a + 1;
    printf("%d\n", a);

exit:
{
    int exit_code = 10;
    printf("Exit Code: %d\n", exit_code);
}

    return 0;
}