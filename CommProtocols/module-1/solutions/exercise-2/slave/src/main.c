/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of I2C in slave mode(CONFIG_I2C_ENABLE_SLAVE_DRIVER_VERSION_2 shall be enabled).
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/stable/esp32c6/api-reference/peripherals/i2c.html
 *
 * @version 0.1
 * @date 2025-02-09
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <time.h>
#include <string.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_sntp.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <driver/gpio.h>
#include <esp_task_wdt.h>
#include <driver/i2c_slave.h>

#define SLAVE I2C_NUM_0
#define PIN_SDA GPIO_NUM_6 // GPIO for SDA
#define PIN_SCL GPIO_NUM_7 // GPIO for SCL
#define ADDRESS 0x55       // Slave Address

#define WIFI_SSID "MaxPlus"
#define WIFI_PASS "SV21TRC3556"

static volatile char command = 0;
static const char *TAG = "Exercise";
static volatile bool request = false;
static volatile bool synchronized = false;

static void time_sync_notification(struct timeval *tv)
{
    synchronized = true;
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

static bool on_request(i2c_slave_dev_handle_t, const i2c_slave_request_event_data_t *, void *)
{
    request = true;
    return request;
}

static bool on_receive(i2c_slave_dev_handle_t, const i2c_slave_rx_done_event_data_t *evt_data, void *)
{
    command = evt_data->buffer[0];
    return true;
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

    i2c_slave_config_t config = {
        .i2c_port = SLAVE,
        .sda_io_num = PIN_SDA,
        .scl_io_num = PIN_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .send_buf_depth = sizeof(struct tm),
        .receive_buf_depth = sizeof(char),
        .slave_addr = ADDRESS,
        .addr_bit_len = I2C_ADDR_BIT_LEN_7,
    };

    i2c_slave_dev_handle_t handle;
    ESP_ERROR_CHECK(i2c_new_slave_device(&config, &handle));

    i2c_slave_event_callbacks_t callbacks = {.on_receive = on_receive, .on_request = on_request};
    i2c_slave_register_event_callbacks(handle, &callbacks, NULL);

    ESP_LOGI(TAG, "I2C Slave Initialized at address 0x%X", ADDRESS);

    // Exclude the Idle Task from the Task WDT
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));

    time_t now;
    uint32_t length;
    struct tm tminfo;

    while (1)
    {
        if (request)
        {
            request = false;

            if (command == 'S')
            {
                command = 0;
                now = time(NULL);

                (void)localtime_r(&now, &tminfo);

                if (!synchronized)
                {
                    tminfo.tm_year = 0;
                    ESP_LOGI(TAG, "Wait on time synchronization...");
                }

                ESP_ERROR_CHECK(i2c_slave_write(handle, (const uint8_t *)&tminfo, sizeof(tminfo), (uint32_t *)&length, 10));
            }
        }
    }
}
