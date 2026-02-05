#include "led.h"
#include "ldr.h"
#include <iostream>

int main(void)
{
    std::cout << led::get_light_max() << std::endl;
    std::cout << ldr::get_resistance_min() << std::endl;
    return 0;
}