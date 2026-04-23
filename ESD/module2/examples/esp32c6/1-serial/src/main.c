/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of serial I/O
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <esp_task_wdt.h>

void app_main(void)
{
    // Exclude the Idle Task from the Task WDT
    //  You can also do it by running: pio run -t menuconfig
    //  Component Config -> ESP System Settings -> Watch CPU0 Idle Task
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));

    int chr;

    while (1)
    {
        chr = getchar();
        if (chr != EOF)
        {
            (void)putchar(chr);
        }
    }
}
