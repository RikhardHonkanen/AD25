/**
 * @file main.cpp
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief
In the microcontroller on Teensy 3.5 there is a Random Number Generator Accelerator unit.

For the details of the RNGA, read chapter 34 of the manual
To get access to the macros representing the registers, include kinetis.h
Develop a driver module(rnga) for the RNGA according to the requirements below:
The driver shall have a function to initialize the RNGA.
Note that the RNGA uses System Clock Gating Control Register 6 (SIM_SCGC6).
The driver shall have a function to start the RNGA
The driver shall have a function to seed the RNGA with an entropy
The driver shall have a function to generate a random integer number in the range of
[min.,max.] The driver shall have a function to stop the RNGA.

 * @version 0.1
 * @date 2026-10-31
 *
 * @copyright Copyright (c) 2026
 *
 */
#include "rnga.h"
#include <Arduino.h>

#define MIN 0U
#define MAX 10U
// #define MIN 0U
// #define MAX 0xFFFFU

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
}

void loop(void)
{
    static uint32_t previous{random(MAX)}; // Initial seed with built in random function
    rnga_init();
    rnga_start();
    rnga_seed(previous);
    uint32_t random = rnga_generate(MIN, MAX);
    Serial.println(random);
    previous = random;
    rnga_end();
    delay(1000);
}