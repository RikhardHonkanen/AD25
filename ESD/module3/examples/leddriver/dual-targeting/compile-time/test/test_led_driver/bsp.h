#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include <stdbool.h>

#define BSP_GPIO_MODE_OUTPUT 2

bool bsp_pin_mode(int pin, int mode);

bool bsp_pin_write(int pin, int state);

// Helper functions
int bsp_get_pin_num(void);
int bsp_get_pin_state(void);
int bsp_get_pin_mode(void);
void bsp_set_mode_status(bool status);
void bsp_set_write_status(bool status);

#endif