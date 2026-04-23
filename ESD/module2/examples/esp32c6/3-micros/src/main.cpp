/**
 * @file main.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A C++ example of timing using the system timer
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <esp_task_wdt.h>
#include <esp_timer.h>
#include <stdio.h>

extern "C" void app_main(void) {
  // Exclude the Idle Task from the Task WDT
  //  You can also do it by running: pio run -t menuconfig
  //  Component Config -> ESP System Settings -> Watch CPU0 Idle Task
  ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));

  uint64_t counter{0};
  int64_t current, prevoius{0};
  constexpr int64_t INTERVAL{500000}; // 1 second

  while (1) {
    current = esp_timer_get_time();

    if (current - prevoius >= INTERVAL) {
      printf("Counter value at %lld μs = %llu\n", current, counter);
      prevoius = current;
    }

    counter++;
  }
}
