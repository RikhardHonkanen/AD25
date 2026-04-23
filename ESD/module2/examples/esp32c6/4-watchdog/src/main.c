/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using the watchdog and esp log
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/system/wdts.html
 *                 https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/system/log.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <esp_log.h>
#include <esp_task_wdt.h>

#define WATCHDOG_TIMEOUT_MS 5000 // 5 seconds timeout

void app_main(void)
{
    // Exclude the Idle Task from the Task WDT
    //  You can also do it by running: pio run -t menuconfig
    //  Component Config -> ESP System Settings -> Watch CPU0 Idle Task
    esp_task_wdt_config_t config = {
        .timeout_ms = WATCHDOG_TIMEOUT_MS,
        .idle_core_mask = 0, // i.e. do not watch the idle task
        .trigger_panic = true,
    };
    ESP_ERROR_CHECK(esp_task_wdt_reconfigure(&config));

    // Add the current task which is the idle task to the watchdog list
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));

    // Feed the watchdog to reset its countdown
    ESP_ERROR_CHECK(esp_task_wdt_reset());

    // Simulate a task
    ESP_LOGI("WATCHDOG", "Task started!");
    for (volatile int i = 0; i < 0x7FFFFF; i++)
    {
        asm("nop");
    }
    ESP_LOGI("WATCHDOG", "Task performed!");

    /* Waiting for the SoC reset. */
    printf("Waiting for reset...\n");

    while (1)
    {
        ;
    }
}