#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include <stdbool.h>

#define BSP_GPIO_MODE_OUTPUT 2

bool bsp_pin_mode(int pin, int mode);

bool bsp_pin_write(int pin, int state);

#endif