#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include <stdbool.h>
#include <driver/gpio.h>

#define BSP_PIN_INPUT_OUTPUT GPIO_MODE_INPUT_OUTPUT

static inline bool bsp_pin_reset(int pin) { return (ESP_OK == gpio_reset_pin(pin)); }

static inline bool bsp_pin_set_direction(int pin, int mode) { return (ESP_OK == gpio_set_direction(pin, mode)); }

static inline bool bsp_pin_set_level(int pin, int level) { return (ESP_OK == gpio_set_level(pin, level)); }

static inline int bsp_pin_get_level(int pin) { return gpio_get_level(pin); }

#endif