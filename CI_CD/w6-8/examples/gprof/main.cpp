#include <cstdio>

void public_func(void);
static void private_func(void);

int main(void)
{
    printf("In main\n");

    public_func();

    return 0;
}

void public_func(void)
{
    int i = 0;

    printf("In public_func\n");

    for (i = 0; i < 50; i++)
    {
        private_func();
    }
}

static void private_func(void)
{
    printf("In private_func\n");

    for (volatile int i = 0; i < 10000000; i++)
    {
        ;
    }
}
