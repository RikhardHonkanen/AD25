/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A Provisioning example using SmartConfig and EspTouch V2.
 *        SmartConfig is a provisioning technique which uses broadcast UDP packets to transfer the SSID and password to connect
 *        the WiFi module to the Internet. To use this provisioning method on ESP32 you need to install Espressif EspTouch
 *        on your phone in order to transfer the SSID and password of the network your phone is connected to, to ESP32.
 *        Look at https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_smartconfig.html
 *
 * @version 0.1
 * @date 2025-08-06
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_eap_client.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_smartconfig.h"
#include "esp_sntp.h"

#define TAG "Example"
#define CONNECTED BIT0
#define SMARTCONFIG_DONE BIT1
#define SYNCHRONIZATION_DONE BIT2

static EventGroupHandle_t wifi_event_group;

static void time_sync_notification(struct timeval *)
{
    xEventGroupSetBits(wifi_event_group, SYNCHRONIZATION_DONE);
}

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        ESP_ERROR_CHECK(esp_smartconfig_set_type(SC_TYPE_ESPTOUCH_V2));
        smartconfig_start_config_t cfg = {
            .esp_touch_v2_enable_crypt = true,
            .esp_touch_v2_key = "16ByteSecretKey!", // The key length shall be 16 bytes
        };
        ESP_ERROR_CHECK(esp_smartconfig_start(&cfg));
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        esp_wifi_connect();

        xEventGroupClearBits(wifi_event_group, CONNECTED);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        xEventGroupSetBits(wifi_event_group, CONNECTED);
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD)
    {
        ESP_LOGI(TAG, "Got SSID and Password");

        smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;

        wifi_config_t wifi_config = {0};
        uint8_t ssid[sizeof(evt->ssid) + 1] = {0};
        uint8_t password[sizeof(evt->password) + 1] = {0};

        memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
        memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));

        memcpy(ssid, evt->ssid, sizeof(evt->ssid));
        memcpy(password, evt->password, sizeof(evt->password));

        ESP_LOGI(TAG, "SSID: %s", ssid);
        ESP_LOGI(TAG, "PASSWORD: %s", password);

        uint8_t rvd_data[16] = {0};
        ESP_ERROR_CHECK(esp_smartconfig_get_rvd_data(rvd_data, sizeof(rvd_data)));
        ESP_LOGI(TAG, "RVD_DATA: %s", rvd_data);

        ESP_ERROR_CHECK(esp_wifi_disconnect());
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
        esp_wifi_connect();
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE)
    {
        xEventGroupSetBits(wifi_event_group, SMARTCONFIG_DONE);
    }
    else
    {
        ;
    }
}

static void wifi_initialize(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    assert(NULL != esp_netif_create_default_wifi_sta());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
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

    wifi_initialize();

    while (1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(wifi_event_group,
                                                 CONNECTED | SMARTCONFIG_DONE | SYNCHRONIZATION_DONE,
                                                 pdTRUE, pdFALSE, portMAX_DELAY);

        if (uxBits & CONNECTED)
        {
            ESP_LOGI(TAG, "WiFi Connected to AP");

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

        if (uxBits & SMARTCONFIG_DONE)
        {
            ESP_LOGI(TAG, "SmartConfig Done");
            esp_smartconfig_stop();
        }

        if (uxBits & SYNCHRONIZATION_DONE)
        {
            ESP_LOGI(TAG, "Time Synchronization Done");
            break;
        }
    }

    time_t now;
    struct tm *tminfo;
    char datetime[20] = {0};

    while (1)
    {
        now = time(NULL);
        tminfo = localtime(&now);

        // Look at: https://en.cppreference.com/w/c/chrono/strftime
        strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", tminfo);
        printf("The current datetime: %s\n", datetime);

        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second delay
    }
}