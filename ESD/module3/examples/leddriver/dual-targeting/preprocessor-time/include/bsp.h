#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include <stdbool.h>
#include <driver/gpio.h>

#define BSP_PIN_INPUT_OUTPUT GPIO_MODE_INPUT_OUTPUT

#define bsp_pin_config(pin, mode) ((ESP_OK == gpio_reset_pin(pin)) && (ESP_OK == gpio_set_direction(pin, mode)))

#define bsp_pin_write(pin, state) (ESP_OK == gpio_set_level(pin, state))

#define bsp_pin_read(pin) gpio_get_level(pin)

#endif