#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include <stdbool.h>

#define BSP_LOW 0
#define BSP_HIGH 1
#define BSP_PULL_UP 0
#define BSP_MODE_INPUT 1

bool bsp_pin_mode(int pin, int mode, int pull);

int bsp_pin_read(int pin);

// Helper functions
int bsp_get_pin_num(void);
int bsp_get_pin_mode(void);
int bsp_get_pin_pull(void);
void bsp_set_pin_state(int state);
void bsp_set_mode_status(bool status);

#endif /* BSP_H */
