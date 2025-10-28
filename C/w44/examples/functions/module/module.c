#include <stdio.h>
#include "module.h"

static void hello(void);

void hello_extern(void);

void print(int x, int y, char operation, int z)
{
    printf("%d %c %d = %d\n", x, operation, y, z);
    hello();
}

static void hello(void)
{
    printf("Hello World!\n");
}

void hello_extern(void)
{
    printf("Hello Extern!\n");
}
