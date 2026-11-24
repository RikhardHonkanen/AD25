/**
 * @file main.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of I2C in slave mode. For the connections look at README.md
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
#define ADDRESS 0x55

static uint8_t buffer[MSGLEN + 1] = {0};

void requestEvent(void)
{
    if (MSGLEN != Wire.write(buffer, MSGLEN))
    {
        Serial.println("Failed");
    }
}

void receiveEvent(size_t length)
{
    Serial.print("Received: ");

    if (length == MSGLEN)
    {
        if (length == Wire.readBytes(buffer, length))
        {
            Serial.print((const char *)buffer);
            for (int i = 0; i < MSGLEN; i++)
            {
                buffer[i] = toupper(buffer[i]);
            }
        }
    }

    Serial.println();
}

void setup()
{
    Serial.begin(9600);

    Wire.begin(ADDRESS);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
}

void loop()
{
}
