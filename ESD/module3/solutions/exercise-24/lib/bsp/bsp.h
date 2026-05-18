#ifndef BSP_H
#define BSP_H

#include <stdbool.h>

#define BSP_CONSOLE_EOF -1
#define BSP_GPIO_MODE_OUTPUT 3

int bsp_console_getchar(void);

bool bsp_console_print(const char *str);

bool bsp_pin_mode(int pin, int mode);

bool bsp_pin_write(int pin, int state);

#endif