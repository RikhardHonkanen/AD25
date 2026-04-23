#include "gpio.h"

#define PIN_DISABLE 0
#define PIN_INVALID -1

static int pin_level = ~0;
static bool pin_rst_status = false;
static bool pin_dir_status = false;
static bool pin_lvl_status = false;
static gpio_num_t pin_num = PIN_INVALID;
static gpio_mode_t pin_mode = PIN_DISABLE;

esp_err_t gpio_reset_pin(gpio_num_t gpio_num)
{
    pin_num = gpio_num;
    return (pin_rst_status ? ESP_OK : ESP_FAIL);
}

esp_err_t gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode)
{
    pin_mode = mode;
    pin_num = gpio_num;
    return (pin_dir_status ? ESP_OK : ESP_FAIL);
}

esp_err_t gpio_set_level(gpio_num_t gpio_num, uint32_t level)
{
    pin_level = level;
    pin_num = gpio_num;
    return (pin_lvl_status ? ESP_OK : ESP_FAIL);
}

int gpio_get_level(gpio_num_t gpio_num)
{
    pin_num = gpio_num;
    return (pin_lvl_status ? pin_level : !pin_level);
}

int gpio_get_pin_num(void)
{
    return pin_num;
}

int gpio_get_pin_mode(void)
{
    return pin_mode;
}

void gpio_set_dir_status(bool status)
{
    pin_dir_status = status;
}

void gpio_set_level_status(bool status)
{
    pin_lvl_status = status;
}

void gpio_set_reset_status(bool status)
{
    pin_rst_status = status;
}