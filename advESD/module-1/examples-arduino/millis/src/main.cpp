#include <Arduino.h>

static uint8_t led_state = LOW;
static uint32_t last_millis = 0;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    uint32_t now = millis();

    if (now - last_millis >= 500u)
    {
        last_millis = now;
        led_state = !led_state;
        digitalWrite(LED_BUILTIN, led_state);
    }
}
