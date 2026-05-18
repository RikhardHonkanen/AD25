#ifndef BSP_H
#define BSP_H

#include <stdbool.h>

#define BSP_CONSOLE_EOF -1
#define BSP_CONSOLE_BUFSIZE 64
#define BSP_GPIO_MODE_OUTPUT 3

int bsp_console_getchar(void);

bool bsp_console_print(const char *str);

bool bsp_pin_mode(int pin, int mode);

bool bsp_pin_write(int pin, int state);

// Helper functions
int bsp_get_pin_num(void);
int bsp_get_pin_state(void);
int bsp_get_pin_mode(void);
void bsp_set_mode_status(bool status);
void bsp_set_write_status(bool status);

void bsp_console_set_input(char chr);
const char *bsp_console_get_output(void);
void bsp_console_set_status(bool status);

#endif