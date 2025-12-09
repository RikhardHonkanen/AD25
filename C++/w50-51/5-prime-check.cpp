/**
 * @file 5-prime-check.cpp
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a program to get a positive integer as an argument from the command line and then check if the number is prime or not.
 * @version 0.1
 * @date 2025-12-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "Program: " << argv[0] << std::endl;
    std::cout << "Argument 1: " << argv[1] << std::endl;

    int num = std::stoi(argv[1]);
    bool is_prime = true;

    if (num == 2)
    {
    }
    else if (num < 2)
    {
        std::cout << num << " is not prime (less than 2)." << std::endl;
        is_prime = false;
    }
    else
    {
        for (int i{3}; i * i <= num; i += 2)
        {
            if (num % i == 0)
            {
                std::cout << num << " is not prime (divisible by " << i << ")." << std::endl;
                is_prime = false;
            }
        }
    }
    if (is_prime)
    {
        std::cout << num << " is prime!" << std::endl;
    }

    return 0;
};