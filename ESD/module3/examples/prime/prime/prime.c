#include <math.h>
#include "prime.h"

#define FIRST_PRIME 2U

bool prime_check(uint32_t number)
{
    bool result = true;

    if (number < FIRST_PRIME)
    {
        result = false;
    }
    else
    {
        if (number > FIRST_PRIME)
        {
            uint8_t counter = FIRST_PRIME;
            uint8_t max_number = ceil(sqrt(number));

            while (counter <= max_number)
            {
                if (number % counter == 0)
                {
                    result = false;
                    break;
                }

                counter++;
            }
        }
    }

    return result;
}
