/**
 * @file exercise-36.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief The starter of Exercise 36.
 * @version 0.1
 * @date 2021-03-04
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#define ARRAY_MAX_LEN 16U
#define POLYNOMIAL 0xC599U
#define MSB_ONE ((uint16_t)0x8000U)
#define CRC_ERROR ((uint16_t)0xFFFFU)

// Calculate the CRC
static uint16_t get_crc15(const uint8_t *data, uint8_t size)
{
    uint16_t remainder = CRC_ERROR;

    if ((data != NULL) && (size > sizeof(remainder)) && (size <= ARRAY_MAX_LEN))
    {
        remainder = 0;

        // Complete the code
    }

    return remainder;
}

// Replace the padded zeros with the CRC accroding to the requirements.
static void checksum(uint8_t *data, uint8_t size, uint16_t crc)
{
    if ((data != NULL) && (size > sizeof(crc)) && (size <= ARRAY_MAX_LEN))
    {
        // Complete the code
    }
}

int main(void)
{
    uint8_t message[14] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', 0, 0}; // Instead of 15 zeros, 16 zeros have been appended to the message.
                                                                                              // We can assume that the MSB of message[12], which is a zero,
                                                                                              // is a bit of the data.

    uint16_t crc = get_crc15(message, sizeof(message)); // Calculate the CRC.

    if (crc != CRC_ERROR)
    {
        (void)printf("The CRC of the message is: 0x%X\n", crc); // For example the CRCs of "Hello World!" is 0xB35 and "AB" is 0x54FB

        checksum(message, sizeof(message), crc);

        if (0 == get_crc15(message, sizeof(message)))
        {
            (void)printf("The data is OK\n");
        }
        else
        {
            (void)printf("The data is not OK\n");
        }

        message[1] = 'a';

        if (0 == get_crc15(message, sizeof(message)))
        {
            (void)printf("The data is OK\n");
        }
        else
        {
            (void)printf("The data is not OK\n");
        }
    }
    else
    {
        (void)printf("Invalid arguments!\n");
    }

    return 0;
}
