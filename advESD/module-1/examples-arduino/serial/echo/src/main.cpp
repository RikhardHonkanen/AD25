#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
}

void loop()
{
  if (0 < Serial.available())
  {
    Serial.printf("%c", Serial.read());
  }
}
