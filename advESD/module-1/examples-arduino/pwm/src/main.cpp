#include <Metro.h>
#include <Arduino.h>

#define STEP 5
#define DELAY 50

static int8_t fade_amount = STEP;
static int16_t brightness = 0;

void setup()
{
}

void loop()
{
    delay(DELAY);

    analogWrite(A9, brightness);

    brightness += fade_amount;

    if ((brightness == 0) || (brightness == 255))
    {
        fade_amount = -fade_amount;
    }
}
