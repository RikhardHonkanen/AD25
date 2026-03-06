/**
 * @file 13-luhn-algorithm.cpp
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief The Luhn algorithm, also known as modulus 10 algorithm, is a simple checksum method
 * used to detect errors in a variety of identification numbers, such as credit card numbers
 * and Swedish personal identity numbers (personnummer).
 *
 * In a 10-digit personnummer (YYMMDD-nnnC), YYMMDD is the birth date, nnn is a serial number
 * which is called birth number and the last digit (C) is a control digit. The control digit is
 * calculated:
 *
 * The numbers in the birth date and the birth number are alternately multiplied by 2 and 1.
 * Sum all the digits in each product.
 * If a product is larger than 9, each digit in the number is used.
 * For example, 13 will be 1 + 3
 * The first digit of the sum is subtracted from 10
 * The remainder of the division of the result of the previous step by 10 is the control bit.
 * For example: 811218-987
 * 8×2 + 1×1 + 1×2 + 2×1 + 1×2 + 8×1 + 9×2 + 8×1 + 7×2
 * 16        1        2         2        2        8       18        8       14
 * (1+6) + 1   +  2   +    2   +   2  +   8  + (1+8) +  8   + (1+4) = 44
 * The control digit (C) is (10 - (44 % 10)) % 10 = 6
 * So the personnummer is  811218-9876
 * Make a program to read personal identity numbers (personnummer) in the format of YYMMDD-nnnC
 * from the terminal and check if the personnummer is correct or not. You need to calculate the
 * control digit of the personnummer and compare it with the last digit of what the user has entered.
 * If the calculated one and the one entered by the user are the same, it means that the personnummer
 * is correct.
 * Otherwise, the entered personnummer is not a valid personnummer. Please note that the format of
 * the personnummer shall also be checked.
 * @version 0.1
 * @date 2025-12-16
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>
#include <string>

constexpr int HYPHEN_IDX{6};
constexpr int PNR_LENGTH{11};

bool validate_format(const std::string pnr)
{
    int length = pnr.length();
    bool status = true;
    if (length != PNR_LENGTH)
    {
        status = false;
    }
    else
    {
        for (size_t i{0}; i < length; i++)
        {
            if (i == HYPHEN_IDX)
            {
                if (pnr.at(i) != '-')
                {
                    status = false;
                }
            }
            else if (!isdigit(pnr.at(i)))
            {
                status = false;
            }
        }
    }
    return status;
}

bool validate_control_digit(const std::string pnr)
{
    bool status = true;
    bool mul_two = true;
    int total = 0;

    for (int i{0}; i < pnr.length() - 1; i++)
    {
        if (pnr.at(i) == '-')
        {
            continue;
        }

        int num = static_cast<int>(pnr.at(i) - '0');
        if (mul_two)
        {
            num *= 2;
            if (num >= 10)
            {
                total += (num % 10) + 1;
            }
            else
            {
                total += num;
            }
        }
        else
        {
            total += num;
        }

        mul_two = !mul_two;
    }
    int comp = static_cast<int>(pnr.back() - '0');
    int control_digit = (10 - (total % 10)) % 10;
    if (!(comp == control_digit))
    {
        status = false;
    }

    return status;
}

int main(void)
{
    std::string user_input;
    std::cout << "Enter pnr in format xxxxxx-xxxx: \n";
    std::getline(std::cin, user_input);

    bool valid = false;
    if (validate_format(user_input))
    {
        valid = validate_control_digit(user_input);
    }

    std::string msg = valid ? "valid" : "not valid";

    std::cout << "The pnr is " << msg << "." << std::endl;

    return 0;
};