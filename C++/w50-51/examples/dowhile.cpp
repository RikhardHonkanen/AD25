#include <iostream>

int main(void)
{
    char command;

#if 0
    std::cout << "Press A: ";
    std::cin >> command;

    while (command != 'A')
    {
        std::cout << "Press A: ";
        std::cin >> command;
    }
#endif

    do
    {
        std::cout << "Press A: ";
        std::cin >> command;
    } while (command != 'A');

    return 0;
}