/**
 * @file exercise-5.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to get a positive integer as an argument from the command line
 *        and then check if the number is prime or not.
 *
 *        To compile and run: g++ exercise-22.cpp -lm -o main && ./main 123
 *
 * @version 0.1
 * @date 2022-11-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cmath>
#include <iostream>

constexpr int FIRST_PRIME{2};

static bool isprime(int num)
{
    bool prime{true};

    if (num < FIRST_PRIME)
    {
        prime = false;
    }
    else
    {
        if (num > FIRST_PRIME)
        {
            const int max_number = std::ceil(std::sqrt(num));

            for (int i = FIRST_PRIME; i <= max_number; i++)
            {
                if (num % i == 0)
                {
                    prime = false;
                    break;
                }
            }
        }
    }

    return prime;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Invalid number of arguments." << std::endl;
        exit(1);
    }

    int number = std::stoi(argv[1]);

    std::cout << number << " is " << (isprime(number) ? "" : "not ") << "prime!" << std::endl;

    return 0;
}