#include <iostream>

int main(void)
{
    int i{0};

    while (i < 100)
    {
        i++;

        if ((i == 50) || (i == 75))
        {
            continue;
        }
        std::cout << i << "\t";
    }

    i = 1;

    while (i <= 10)
    {
        int j{1};

        while (j <= 10)
        {
            std::cout << i * j << "\t";
            j++;
        }
        std::cout << std::endl;

        i++;
    }

    return 0;
}