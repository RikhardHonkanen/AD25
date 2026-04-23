#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdbool.h>

typedef struct
{
    struct
    {
        int num;
        int mode;
    } pin;
    int (*get_state)(int);
    bool (*set_state)(int, int);
    bool (*set_mode)(int, int);
} led_driver_config_t;

/**
 * @brief This function is used to initialize the driver.
 *
 * @param ptr A pointer to an instance of led_driver_config_t
 * @return bool true if the pin is initialized successfully; otherwise false.
 */
bool led_driver_init(const led_driver_config_t *ptr);

/**
 * @brief This function is used to change the state of the LED.
 *
 * @param state The set state. It can be 0 or 1.
 * @return bool true if state is set successfully; otherwise false.
 */
bool led_driver_set_state(int state);

#endif