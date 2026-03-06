#include "today.h"
#include <iostream>

int main(void)
{
    Today today;

    std::cout << "Is today (" << today.getDate() << ") a leap day? "
              << (today.isLeapDay() ? "Yes" : "No") << std::endl;

    return 0;
}
