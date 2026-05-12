#ifndef BSP_H
#define BSP_H

#include <stdbool.h>
#include <stdint.h>

#define BSP_LOW 0
#define BSP_HIGH 1
#define BSP_PULL_UP 0
#define BSP_MODE_INPUT 1
#define BUTTON_RELEASED (1U)

bool bsp_pin_mode(int pin, int mode, int pull);

int bsp_pin_read(int pin);

// Helper functions
int bsp_get_pin_num(void);
int bsp_get_pin_state(void);
void bsp_set_pin_state(int state);
int bsp_get_pin_mode(void);
void bsp_set_mode_status(bool status);

#endif /* BSP_H */
