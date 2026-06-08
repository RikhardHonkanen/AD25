#include <time.h>
#include <stdio.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_sntp.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <driver/gpio.h>
#include <driver/uart.h>

#define UART UART_NUM_1                  // Using UART1
#define TX_PIN GPIO_NUM_5                // Define TX pin
#define RX_PIN GPIO_NUM_4                // Define RX pin
#define BUF_SIZE (2 * SOC_UART_FIFO_LEN) // Buffer size shall be greater than SOC_UART_FIFO_LEN

#define WIFI_SSID "MaxPlus"
#define WIFI_PASS "SV21TRC3556"

static const char *TAG = "Exercise";
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

static void uart_initialize(void)
{
    uart_config_t config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Install driver and configure UART
    ESP_ERROR_CHECK(uart_driver_install(UART, BUF_SIZE, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART, &config));
    ESP_ERROR_CHECK(uart_set_pin(UART, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    ESP_LOGI(TAG, "UART initialized");
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

    ESP_LOGI(TAG, "UART Initialization");
    uart_initialize();

    time_t now;
    struct tm tminfo;
    uint8_t buffer[BUF_SIZE] = {0};

    while (1)
    {
        if (sizeof(char) == uart_read_bytes(UART, buffer, sizeof(char), 1))
        {
            if (buffer[0] == 'S')
            {
                now = time(NULL);

                (void)localtime_r(&now, &tminfo);

                if (!synchronized)
                {
                    tminfo.tm_year = 0;
                    ESP_LOGI(TAG, "Wait on time synchronization...");
                }

                memcpy(buffer, &tminfo, sizeof(tminfo));
                if (sizeof(tminfo) != uart_write_bytes(UART, buffer, sizeof(tminfo)))
                {
                    ESP_LOGE(TAG, "Failed to write");
                }
            }
        }
    }
}
