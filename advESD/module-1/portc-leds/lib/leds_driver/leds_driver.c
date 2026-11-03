#include <stdlib.h>
#include <limits.h>
#include "leds_driver.h"

static volatile uint8_t *address = NULL;

uint8_t leds_driver_begin(volatile uint8_t *leds_address)
{
    uint8_t status = LEDS_DRIVER_ERROR;

    if (leds_address != NULL)
    {
        status = LEDS_DRIVER_OK;
        address = leds_address;
        *address = LEDS_DRIVER_ALL_OFF;
    }

    return status;
}

uint8_t leds_driver_get_state(uint8_t led_number)
{
    uint8_t result = LEDS_DRIVER_ERROR;

    if ((address != NULL) && (led_number < CHAR_BIT))
    {
        uint8_t mask = (1 << led_number);
        result = ((*address & mask) == 0) ? LEDS_DRIVER_OFF : LEDS_DRIVER_ON;
    }

    return result;
}

uint8_t leds_driver_turn_on(uint8_t led_number)
{
    uint8_t status = LEDS_DRIVER_ERROR;

    if ((address != NULL) && (led_number < CHAR_BIT))
    {
        *address |= (1 << led_number);
        status = LEDS_DRIVER_OK;
    }

    return status;
}

uint8_t leds_driver_turn_off(uint8_t led_number)
{
    uint8_t status = LEDS_DRIVER_ERROR;

    if ((address != NULL) && (led_number < CHAR_BIT))
    {
        uint8_t mask = ~(1 << led_number);
        *address &= mask;
        status = LEDS_DRIVER_OK;
    }

    return status;
}

uint8_t leds_driver_turn_all_on(void)
{
    uint8_t status = LEDS_DRIVER_ERROR;

    if (address != NULL)
    {
        *address = LEDS_DRIVER_ALL_ON;
        status = LEDS_DRIVER_OK;
    }

    return status;
}

uint8_t leds_driver_turn_all_off(void)
{
    uint8_t status = LEDS_DRIVER_ERROR;

    if (address != NULL)
    {
        *address = LEDS_DRIVER_ALL_OFF;
        status = LEDS_DRIVER_OK;
    }

    return status;
}

void leds_driver_end(void)
{
    if (address != NULL)
    {
        *address = LEDS_DRIVER_ALL_ON;
        address = NULL;
    }
}