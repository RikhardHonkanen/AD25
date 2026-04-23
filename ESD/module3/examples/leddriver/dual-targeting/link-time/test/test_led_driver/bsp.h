#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include <stdbool.h>

#define BSP_PIN_INPUT_OUTPUT 3

bool bsp_pin_config(int pin, int mode);

bool bsp_pin_write(int pin, int state);

int bsp_pin_read(int pin);

// Helper functions
int bsp_get_pin_num(void);
int bsp_get_pin_mode(void);
void bsp_set_config_status(bool status);
void bsp_set_state_status(bool status);

#endif