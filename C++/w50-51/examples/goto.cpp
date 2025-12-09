#include <iostream>

int main(void)
{
    int a{10};

loop:
    std::cout << a << std::endl;

    a--;
    if (a > 0)
    {
        goto loop;
    }

    goto exit;

    a = 2 * a + 1;
    std::cout << a << std::endl;

exit:
{
    int code{10};
    std::cout << "Exit Code: " << code << std::endl;
}

    return 0;
}