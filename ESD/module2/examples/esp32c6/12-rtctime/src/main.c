/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of setting and getting datetime of the internal RTC using the functions in posix standard time.h
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/system/system_time.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <esp_sntp.h>

void app_main(void)
{
    struct tm timeinfo = {
        .tm_sec = 10,
        .tm_min = 10,
        .tm_hour = 10,
        .tm_mday = 21,
        .tm_mon = 3 - 1,
        .tm_year = 2025 - 1900,
    }; // 2025-03-21 10:10:10

    struct timeval now = {0};
    now.tv_sec = mktime(&timeinfo);
    settimeofday(&now, NULL);

    while (1)
    {
        time_t now = time(NULL);
        char datetime[32] = {0};
        struct tm *timeinfo = localtime(&now);

        // For details of strftime look at: https://en.cppreference.com/w/c/chrono/strftime
        strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", timeinfo);

        printf("The current datetime: %s\n", datetime);

        sleep(1); // 1 second delay
    }
}
