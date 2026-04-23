#include <unistd.h>
#include <assert.h>
#include "led_driver.h"
#include <driver/gpio.h>

static inline bool pin_config(int pin, int mode) { return ((ESP_OK == gpio_reset_pin(pin)) && (ESP_OK == gpio_set_direction(pin, mode))); }
static inline bool pin_set_state(int pin, int state) { return (ESP_OK == gpio_set_level(pin, state)); }
static inline int pin_get_state(int pin) { return gpio_get_level(pin); }

void app_main(void)
{
    led_driver_config_t config = {
        .pin.num = GPIO_NUM_4,
        .pin.mode = GPIO_MODE_INPUT_OUTPUT,
        .get_state = pin_get_state,
        .set_state = pin_set_state,
        .set_mode = pin_config};
    assert(led_driver_init(&config));

    int state = 0;

    while (1)
    {
        state = !state;

        assert(led_driver_set_state(state));

        sleep(1); // 1s delay
    }
}
