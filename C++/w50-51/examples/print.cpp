#include <iostream>
#include <unistd.h>

int main(void)
{
    std::cout << ">> Print\tHorizontalTAB" << std::endl;
    std::cout << ">> Print\vVerticalTAB" << std::endl;
    std::cout << ">> Print\na new line." << std::endl;
    std::cout << ">> Printt\bBackspace" << std::endl;
    std::cout << ">> Print special characters: % \' \" \\" << std::endl;

    for (size_t i = 0; i < 15; i++)
    {
        std::cout << "\r" << i << std::flush;
        usleep(100000);
    }

    std::cout << std::endl;

    return 0;
}