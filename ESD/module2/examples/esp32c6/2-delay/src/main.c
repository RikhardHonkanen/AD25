/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of timing using unistd usleep
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <unistd.h>

#define DELAY 500000 // 1000000µs = 1s

void app_main(void) {
  while (1) {
    printf("Hello there!\n"); // Print to the terminal via serial

    usleep(DELAY); // 1 second delay
  }
}
