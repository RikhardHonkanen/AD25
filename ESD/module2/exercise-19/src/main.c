#include <time.h>
#include "wifi.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "esp_log.h"
#include "esp_sntp.h"

static const char *TAG = "Exercise";

void time_sync_notification(struct timeval *tv)
{
    struct tm tminfo = {0};
    (void)localtime_r(&tv->tv_sec, &tminfo);
    ESP_LOGI(TAG, "Time is set: %s", asctime(&tminfo));
}

static void ntp_init(void)
{
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_set_time_sync_notification_cb(time_sync_notification);
    esp_sntp_init();

    setenv("TZ", "CET-1CEST,M3.5.0/02:00:00,M10.5.0/03:00:00", 1);
    tzset();
}

void app_main(void)
{
    ESP_ERROR_CHECK(wifi_init());

    while (!wifi_connected())
    {
        putchar('.');
        usleep(50000);

        char chr = toupper(getchar());

        if (chr == 'R')
        {
            wifi_reset();
        }
    }

    ntp_init();

    time_t now;
    struct tm *tminfo;
    char datetime[20] = {0};
    const int compile_year = atoi(__DATE__ + 7) - 1900;

    while (1)
    {
        now = time(NULL);
        tminfo = localtime(&now);

        if (tminfo->tm_year < compile_year)
        {
            ESP_LOGI(TAG, "Wait on time synchronization...");
        }
        else
        {
            // Look at: https://en.cppreference.com/w/c/chrono/strftime
            strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", tminfo);
            printf("The current datetime: %s\n", datetime);
        }

        sleep(1); // 1 second delay
    }
}