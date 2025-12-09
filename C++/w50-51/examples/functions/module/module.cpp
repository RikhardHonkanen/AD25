#include "module.h"
#include <iostream>

static void hello(void);

void hello_extern(void);

void module_print(int x, int y, char operation, int z)
{
    printf("%d %c %d = %d\n", x, operation, y, z);
    hello();
}

static void hello(void)
{
    std::cout << "Hello World!" << std::endl;
}

void hello_extern(void)
{
    std::cout << "Hello Extern!" << std::endl;
}
