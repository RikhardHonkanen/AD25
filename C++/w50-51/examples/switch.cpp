#include <iostream>

int main(void)
{
    std::cout << "A) Add a file" << std::endl;
    std::cout << "E) Edit a file" << std::endl;
    std::cout << "D) Delete a file" << std::endl;
    std::cout << "Enter a command: ";

    char command;
    std::cin >> command;

    switch (command)
    {
    case 'a':
    case 'A':
        std::cout << "The file added!" << std::endl;
        break;

    case 'e':
    case 'E':
        std::cout << "The file edited!" << std::endl;
        break;

    case 'd':
    case 'D':
        std::cout << "The file deleted!" << std::endl;
        break;

    default:
        std::cout << "Invalid command!" << std::endl;
        break;
    }

    return 0;
}