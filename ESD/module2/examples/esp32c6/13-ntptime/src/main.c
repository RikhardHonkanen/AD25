/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of getting the current datetime from an NTP server and set the internal RTC datetime.
 *        Look at: https://developer.espressif.com/blog/getting-started-with-wifi-on-esp-idf/#part-2-using-the-wi-fi-apis
                   https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/system/system_time.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <time.h>
#include <stdio.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_sntp.h>
#include <esp_event.h>
#include <nvs_flash.h>

#define WIFI_SSID "MaxPlus"
#define WIFI_PASS "SV21TRC3556"

static const char *TAG = "wifi_sntp";

void time_sync_notification(struct timeval *tv)
{
    struct tm tminfo = {0};
    (void)localtime_r(&tv->tv_sec, &tminfo);
    ESP_LOGI(TAG, "Time is set: %s", asctime(&tminfo));
}

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        esp_wifi_connect();
        ESP_LOGI(TAG, "Retrying to connect to the AP");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ESP_LOGI(TAG, "Connected to AP, start SNTP");

        esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
        esp_sntp_setservername(0, "pool.ntp.org");
        esp_sntp_set_time_sync_notification_cb(time_sync_notification);
        esp_sntp_init();

        // https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
        // https://support.cyberdata.net/portal/en/kb/articles/010d63c0cfce3676151e1f2d5442e311
        // E.g. zdump -v /usr/share/zoneinfo/Europe/Stockholm | grep 2025
        setenv("TZ", "CET-1CEST,M3.5.0/02:00:00,M10.5.0/03:00:00", 1);
        tzset();
    }
}

void wifi_initialize(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    assert(NULL != esp_netif_create_default_wifi_sta());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_initialize finished.");
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_initialize();

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
