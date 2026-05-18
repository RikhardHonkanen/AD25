#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdbool.h>

bool led_driver_init(int pin);
bool led_driver_set_state(int state);

// Helper functions
int led_driver_get_pin(void);
int led_driver_get_state(void);
bool led_driver_is_init(void);
void led_driver_set_status(bool status);

#endif