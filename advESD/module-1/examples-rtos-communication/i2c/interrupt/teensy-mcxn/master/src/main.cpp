/**
 * @file main.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of I2C in master mode. For the connections look at README.md
 *        Look at: https://docs.arduino.cc/language-reference/en/functions/communication/wire/#functions
 *
 * @version 0.1
 * @date 2025-10-26
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <i2c_t3.h>
#include <Arduino.h>

#define MSGLEN 8
#define ADDRESS 0x44
#define SLAVE_ADDRESS 0x55
#define I2C_BAUDRATE 400000U

void setup()
{
    Serial.begin(9600);

    Wire.begin(I2C_MASTER, ADDRESS, I2C_PINS_18_19, I2C_PULLUP_INT, I2C_BAUDRATE); // ADDRESS will be ignored
}

void loop()
{
    uint8_t buffer[MSGLEN + 1] = {0};
    for (int i = 0; i < MSGLEN; i++)
    {
        buffer[i] = 'a' + random(26);
    }

    Serial.print("    Sent: ");

    Wire.beginTransmission(SLAVE_ADDRESS);
    if (MSGLEN == Wire.write(buffer, MSGLEN))
    {
        if (0 == Wire.endTransmission())
        {
            Serial.printf("%s\nReceived: ", buffer);

            if (MSGLEN == Wire.requestFrom(SLAVE_ADDRESS, MSGLEN))
            {
                if (MSGLEN == Wire.readBytes(buffer, MSGLEN))
                {
                    Serial.printf("%s\n\n", buffer);
                }
                else
                {
                    Serial.println("Failed\n");
                }
            }
        }
        else
        {
            Serial.println("Failed");
        }
    }
    else
    {
        Serial.println("Failed");
    }

    delay(500);
}
