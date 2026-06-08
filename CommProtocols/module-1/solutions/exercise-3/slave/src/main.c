/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of SPI in slave mode.
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/stable/esp32c6/api-reference/peripherals/spi_slave.html
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
#include <driver/spi_slave.h>

#define WIFI_SSID "MaxPlus"
#define WIFI_PASS "SV21TRC3556"

#define SPI_HOST SPI2_HOST      // SPI Controller
#define SPI_CS_PIN GPIO_NUM_18  // FSPICS2
#define SPI_CLK_PIN GPIO_NUM_6  // FSPICLK
#define SPI_MISO_PIN GPIO_NUM_2 // FSPIIQ
#define SPI_MOSI_PIN GPIO_NUM_7 // FSPID

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

    spi_bus_config_t bus_config = {
        .miso_io_num = SPI_MISO_PIN,
        .mosi_io_num = SPI_MOSI_PIN,
        .sclk_io_num = SPI_CLK_PIN,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
        .max_transfer_sz = sizeof(struct tm),
    };

    // Configuration for the SPI slave interface
    spi_slave_interface_config_t slave_config = {
        .spics_io_num = SPI_CS_PIN,
        .queue_size = 2, // We have 2 transactions
    };

    // Initialize SPI slave interface
    assert(ESP_OK == spi_slave_initialize(SPI_HOST, &bus_config, &slave_config, SPI_DMA_DISABLED));

    ESP_LOGI(TAG, "SPI2 Slave initialized successfully");

    time_t now;
    struct tm tminfo;
    uint8_t command = 0;

    spi_slave_transaction_t tx_trans = {.length = sizeof(tminfo) * CHAR_BIT, .tx_buffer = &tminfo};
    spi_slave_transaction_t rx_trans = {.length = sizeof(command) * CHAR_BIT, .rx_buffer = &command};

    while (1)
    {
        if (ESP_OK == spi_slave_transmit(SPI_HOST, &rx_trans, portMAX_DELAY))
        {
            if (command == 'S')
            {
                now = time(NULL);

                (void)localtime_r(&now, &tminfo);

                if (!synchronized)
                {
                    tminfo.tm_year = 0;
                    ESP_LOGI(TAG, "Wait on time synchronization...");
                }
            }

            if (ESP_OK != spi_slave_transmit(SPI_HOST, &tx_trans, portMAX_DELAY))
            {
                printf("Failed to write\n");
            }
        }
        else
        {
            printf("Failed to read\n");
        }
    }
}