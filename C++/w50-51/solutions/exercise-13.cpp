#include <cctype>
#include <string>
#include <limits>
#include <iostream>

constexpr char FORMAT[]{"YYMMDD-nnnC"};

int main(void)
{
    std::string pid;

    while (1)
    {
        std::cout << "Enter the PID: ";
        std::cin >> pid;

        if (pid == "exit")
        {
            break;
        }
        else if ((sizeof(FORMAT) - 1) == pid.length())
        {
            bool status{true};
            uint8_t digits[sizeof(FORMAT) - 2] = {0};

            for (int i = 0, j = 0; i <= sizeof(digits); i++)
            {
                if (std::isalpha(FORMAT[i]))
                {
                    if (!std::isdigit(pid[i]))
                    {
                        status = false;
                        break;
                    }

                    digits[j++] = pid[i] - '0';
                }
                else
                {
                    if (FORMAT[i] != pid[i])
                    {
                        status = false;
                        break;
                    }
                }
            }

            if (status)
            {
                int sum = 0;
                for (int i = 0; i < sizeof(digits) - 1; i++)
                {
                    int result = digits[i];

                    if (i % 2 == 0)
                    {
                        result *= 2;
                    }

                    sum += result % 10;

                    if (result >= 10)
                    {
                        sum += 1;
                    }
                }

                int control = (10 - (sum % 10)) % 10;

                if (control == digits[sizeof(digits) - 1])
                {
                    std::cout << "The entered PID is valid!"
                              << std::endl
                              << std::endl;
                }
                else
                {
                    std::cout << "The entered PID is not valid! The control digit shall be " << control
                              << std::endl
                              << std::endl;
                }
            }
            else
            {
                std::cout << "The entered PID format is incorrect!"
                          << std::endl
                          << std::endl;
            }
        }
        else
        {
            std::cout << "The entered PID format is incorrect!"
                      << std::endl
                      << std::endl;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return 0;
}