/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of reading the internal temperature sensor.
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/peripherals/temp_sensor.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <driver/temperature_sensor.h>

void app_main(void)
{
    temperature_sensor_handle_t handle = NULL;
    temperature_sensor_config_t config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(10, 50);

    ESP_ERROR_CHECK(temperature_sensor_install(&config, &handle));

    float temperature;

    while (1)
    {
        ESP_ERROR_CHECK(temperature_sensor_enable(handle)); // Enable temperature sensor

        ESP_ERROR_CHECK(temperature_sensor_get_celsius(handle, &temperature)); // Get converted sensor data

        ESP_ERROR_CHECK(temperature_sensor_disable(handle)); // Disable the temperature sensor to save power

        printf("Temperature: %.02f °C\n", temperature);

        sleep(1); // 1 second delay
    }
}
