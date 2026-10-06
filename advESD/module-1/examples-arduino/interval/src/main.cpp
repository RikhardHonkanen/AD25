#include <Arduino.h>
#include <IntervalTimer.h>

static IntervalTimer timer;

void blink(void)
{
  static uint8_t led_state = LOW;
  digitalWrite(LED_BUILTIN, led_state);
  led_state = !led_state;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  timer.begin(blink, 500000); // blink is called every second
}

void loop()
{
}
