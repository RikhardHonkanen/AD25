#include <EEPROM.h>
#include <Arduino.h>

// Uncomment the following line to write a float to the EEPROM
// #define WRITE

void setup()
{
  Serial.begin(9600);
  while (!Serial.dtr())
  {
    delay(100);
  }

  EEPROM.begin();

#ifdef WRITE
  float value{3.14f};
  uint8_t *ptr{reinterpret_cast<uint8_t *>(&value)};
  for (size_t i = 0; i < sizeof(value); i++)
  {
    EEPROM.write(i, ptr[i]);
  }
#endif
}

void loop()
{
  uint8_t temp[sizeof(float)] = {0};

  for (size_t i = 0; i < sizeof(temp); i++)
  {
    temp[i] = EEPROM.read(i);
  }

  float value = *reinterpret_cast<float *>(temp);
  Serial.printf("%f\n", value);

  delay(1000);
}
