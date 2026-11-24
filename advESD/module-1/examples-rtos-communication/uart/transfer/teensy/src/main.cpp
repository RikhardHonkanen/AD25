/**
 * @file main.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of UART communication. For connections look at README.md
 *        Look at: https://docs.arduino.cc/language-reference/en/functions/communication/serial/#functions
 *
 * @version 0.1
 * @date 2025-10-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <Arduino.h>

#define BAUDRATE 115200
#define UART Serial1
#define MSGLEN 8

void setup()
{
    Serial.begin(9600);

    UART.begin(BAUDRATE);
}

void loop()
{
    uint8_t buffer[MSGLEN + 1] = {0};
    for (int i = 0; i < MSGLEN; i++)
    {
        buffer[i] = 'a' + random(26);
    }

    Serial.print("    Sent: ");
    if (MSGLEN == UART.write(buffer, MSGLEN))
    {
        Serial.printf("%s\n", buffer);
    }
    else
    {
        Serial.print("Failed\n");
    }

    Serial.print("Received: ");
    if (MSGLEN == UART.readBytes(buffer, MSGLEN))
    {
        Serial.printf("%s\n", buffer);
    }
    else
    {
        Serial.print("Failed\n");
    }

    Serial.println();
    delay(1000);
}