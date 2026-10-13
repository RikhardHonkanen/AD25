/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of over the air update.
 *
 *        There shall be two OTA partitions:
 *        In platformio.ini add: board_build.partitions = partitions_two_ota.csv
 *        Look at: ~/.platformio/packages/framework-espidf/components/partition_table/partitions_two_ota.csv
 *                 https://docs.espressif.com/projects/esp-idf/en/v5.5/esp32c6/api-reference/system/ota.html
 *
 *        To run the example in terminal:
 *          - To generate the server certificate, run: make certificate
 *          - Then only once, run: make upload
 *          - Then to build and upload the new program over the air, run: make update
 *
 * @version 0.1
 * @date 2025-08-15
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "wifi.h"

#ifdef NEW
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#endif

#define WIFI_SSID "MaxPlus"
#define WIFI_PASS "SV21TRC3556"

#define SERVER_PORT "12345"
#define SERVER_IP "192.168.0.100"

void app_main(void)
{
    wifi_connect(WIFI_SSID, WIFI_PASS);

    wifi_ota_update_set_server(SERVER_IP, SERVER_PORT);

#ifdef NEW
    uint32_t state = 0;
    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_4));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT)); // Configure pin 4 as a digital output pin

    while (1)
    {
        state = !state;
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_4, state));
        vTaskDelay(pdMS_TO_TICKS(500));
    }
#endif
}
