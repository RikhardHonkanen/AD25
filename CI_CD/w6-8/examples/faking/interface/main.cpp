#include "date.h"
#include "today.h"
#include <iostream>

int main(void)
{
    Date date;
    Today today{date};

    std::cout << "Is today (" << today.getDate() << ") a leap day? "
              << (today.isLeapDay() ? "Yes" : "No") << std::endl;

    return 0;
}
