#ifndef LEDS_DRIVER_H
#define LEDS_DRIVER_H

#include <stdint.h>

#define LEDS_DRIVER_ALL_ON 0xFFu
#define LEDS_DRIVER_ALL_OFF 0x00u

#define LEDS_DRIVER_ON 0x01u
#define LEDS_DRIVER_OFF 0x00u

#define LEDS_DRIVER_OK 0xF0u
#define LEDS_DRIVER_ERROR 0x0Fu

#ifdef __cplusplus
extern "C"
{
#endif

    uint8_t leds_driver_begin(volatile uint8_t *leds_address);

    uint8_t leds_driver_get_state(uint8_t led_number);

    uint8_t leds_driver_turn_on(uint8_t led_number);

    uint8_t leds_driver_turn_off(uint8_t led_number);

    uint8_t leds_driver_turn_all_on(void);

    uint8_t leds_driver_turn_all_off(void);

    void leds_driver_end(void);

#ifdef __cplusplus
}
#endif

#endif
