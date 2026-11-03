#include "portc.h"
#include <Arduino.h>
#include "leds_driver.h"

void setup()
{
    portc_init();

    leds_driver_begin(portc_address());
}

void loop(void)
{
    leds_driver_turn_all_on();
    delay(500);

    leds_driver_turn_all_off();
    delay(500);
}