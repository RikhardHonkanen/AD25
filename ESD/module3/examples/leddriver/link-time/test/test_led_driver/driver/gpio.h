#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>

#define ESP_OK 0                 /*!< esp_err_t value indicating success (no error) */
#define ESP_FAIL -1              /*!< Generic esp_err_t code indicating failure */
#define GPIO_MODE_INPUT_OUTPUT 3 /*!< GPIO mode : output and input mode */

typedef int esp_err_t;
typedef int gpio_num_t;
typedef int gpio_mode_t;

esp_err_t gpio_reset_pin(gpio_num_t gpio_num);

esp_err_t gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode);

esp_err_t gpio_set_level(gpio_num_t gpio_num, uint32_t level);

int gpio_get_level(gpio_num_t gpio_num);

// Helper functions
int gpio_get_pin_num(void);
int gpio_get_pin_mode(void);
void gpio_set_dir_status(bool status);
void gpio_set_level_status(bool status);
void gpio_set_reset_status(bool status);

#endif