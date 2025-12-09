/**
 * @file forloop.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of for loop:
 *        Print 5 capitalized (first letter uppercase and the other letters lowercase) random strings in the terminal.
 *        A random string shall only contain letters (a-z and A-z).
 *        Length of a random string shall be a random number in the range of 2 and 8.
 *
 *        No magic numbers!
 *
 *        Example:
 *           Asdh
 *           Bmkdsds
 *           Cpopsp
 *           Dksdsk
 *           Eefansj
 *
 * @version 0.1
 * @date 2022-10-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <ctime>
#include <cstdlib>
#include <iostream>

constexpr int STR_NUM{5};
constexpr int LEN_MIN{2};
constexpr int LEN_MAX{8};
constexpr int ALPHA_NUM{26};

int main(void)
{
    srand(time(nullptr));

    for (int i{0}; i < STR_NUM; ++i)
    {
        int len = LEN_MIN + rand() % (LEN_MAX - LEN_MIN + 1);

        std::cout << static_cast<char>('A' + rand() % ALPHA_NUM);

        for (int j{1}; j < len; j++)
        {
            std::cout << static_cast<char>('a' + rand() % ALPHA_NUM);
        }

        std::cout << std::endl;
    }

    return 0;
}