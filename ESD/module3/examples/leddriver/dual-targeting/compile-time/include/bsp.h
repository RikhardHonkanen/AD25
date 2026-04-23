#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include <stdbool.h>
#include <driver/gpio.h>

#define BSP_PIN_INPUT_OUTPUT GPIO_MODE_INPUT_OUTPUT

static inline bool bsp_pin_config(int pin, int mode) { return ((ESP_OK == gpio_reset_pin(pin)) && (ESP_OK == gpio_set_direction(pin, mode))); }

static inline bool bsp_pin_write(int pin, int state) { return (ESP_OK == gpio_set_level(pin, state)); }

static inline int bsp_pin_read(int pin) { return gpio_get_level(pin); }

#endif