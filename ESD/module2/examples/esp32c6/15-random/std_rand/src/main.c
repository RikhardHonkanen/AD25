/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using the random number generator (RNG) to seed the
 * standard random number generator provided by stdlib Look at:
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/system/random.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <bootloader_random.h>
#include <esp_random.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void random_init(void) {
  bootloader_random_enable();
  srand(esp_random());
  bootloader_random_disable();
}

static void random_get(uint8_t *buf, size_t len) {
  for (size_t i = 0; i < len; i++) {
    buf[i] = rand() % 101;
  }
}

void app_main(void) {
  random_init();

  uint8_t value;

  while (1) {
    random_get((uint8_t *)&value, sizeof(value));

    // printf("Random Number: 0x%08lx\n", value); // Print to the terminal via
    // serial
    printf("Random Number: %d\n", value); // Print to the terminal via serial

    sleep(1); // 1 second delay
  }
}
