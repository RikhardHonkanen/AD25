/**
 * @file main.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of SPI in master mode. For the connections look at README.md
 *        Look at: https://docs.arduino.cc/learn/communication/spi/
 *
 * @version 0.1
 * @date 2025-10-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <SPI.h>
#include <Arduino.h>

#define MSGLEN 8
#define CLOCK_RATE 1000000

static SPISettings setting = SPISettings(CLOCK_RATE, MSBFIRST, SPI_MODE0);

void setup()
{
    pinMode(PIN_SPI_SS, OUTPUT);
    digitalWrite(PIN_SPI_SS, HIGH);

    Serial.begin(9600);
    SPI.begin();
}

void loop()
{
    delay(1000);

    uint8_t buffer[MSGLEN + 1] = {0};

    for (int i = 0; i < MSGLEN; i++)
    {
        buffer[i] = 'a' + random(26);
    }

    Serial.printf("    Sent: %s\n", (char *)buffer);

    SPI.beginTransaction(setting);

    digitalWrite(PIN_SPI_SS, LOW);
    SPI.transfer(buffer, NULL, MSGLEN);
    digitalWrite(PIN_SPI_SS, HIGH);

    delay(1);

    digitalWrite(PIN_SPI_SS, LOW);
    SPI.transfer(NULL, buffer, MSGLEN);
    digitalWrite(PIN_SPI_SS, HIGH);

    SPI.endTransaction();

    Serial.printf("Received: %s\n", (char *)buffer);
    Serial.println();
}