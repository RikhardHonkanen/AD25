/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using the random number generator (RNG) to seed the Counter mode Deterministic Random Bit Generator provided by mbedtls
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/system/random.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <esp_random.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <bootloader_random.h>

static mbedtls_ctr_drbg_context ctr_drbg;

static void random_init(void)
{
    uint32_t seed[8];
    static mbedtls_entropy_context entropy;

    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    bootloader_random_enable();
    for (size_t i = 0; i < sizeof(seed) / sizeof(seed[0]); i++)
    {
        seed[i] = esp_random();
    }
    bootloader_random_disable();

    assert(0 == mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const uint8_t *)seed, sizeof(seed)));
}

static void random_get(uint8_t *buf, size_t len)
{
    assert(0 == mbedtls_ctr_drbg_random(&ctr_drbg, buf, len));
}

void app_main(void)
{
    random_init();

    uint32_t value;

    while (1)
    {
        random_get((uint8_t *)&value, sizeof(value));

        printf("Random Number: 0x%08lx\n", value); // Print to the terminal via serial

        sleep(1); // 1 second delay
    }
}
