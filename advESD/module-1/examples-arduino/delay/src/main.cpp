#include <Arduino.h>

static constexpr int DELAY = 500;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
}

void loop()
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  Serial.println("Hello WOrld!");

  delay(DELAY);
}
