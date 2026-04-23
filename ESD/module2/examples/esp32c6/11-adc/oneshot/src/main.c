/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using ADC1_CH0 (GPIO0) to read the ambient temperature using TMP36.
 *        Please ensure that the connections are correct.
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/peripherals/adc_oneshot.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <esp_log.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali_scheme.h>

#define MILLIVOLT_OFFSET 500.0
#define MILLIVOLT_PER_CELSIUS 10

#define ADC_ATTENUATION ADC_ATTEN_DB_12
#define ADC_RESOLUTION ADC_BITWIDTH_DEFAULT

const static char *TAG = "Example";

void app_main(void)
{
    ESP_LOGI(TAG, "ADC1 Initialization");
    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t adc_config = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc_config, &adc_handle));

    ESP_LOGI(TAG, "ADC1 Configuration");
    adc_oneshot_chan_cfg_t channel_config = {
        .bitwidth = ADC_RESOLUTION,
        .atten = ADC_ATTENUATION,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_0, &channel_config));

    ESP_LOGI(TAG, "ADC1 Calibration");
    adc_cali_handle_t cali_handle = NULL;
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .chan = ADC_CHANNEL_0,
        .atten = ADC_ATTENUATION,
        .bitwidth = ADC_RESOLUTION,
    };
    ESP_ERROR_CHECK(adc_cali_create_scheme_curve_fitting(&cali_config, &cali_handle));

    int adc_raw, millivolts;

    while (1)
    {
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL_0, &adc_raw));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, ADC_CHANNEL_0, adc_raw);

        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle, adc_raw, &millivolts));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Cali Voltage: %d mV", ADC_UNIT_1 + 1, ADC_CHANNEL_0, millivolts);

        printf("Temperature: %.02f °C\n\n", (millivolts - MILLIVOLT_OFFSET) / MILLIVOLT_PER_CELSIUS);

        sleep(1); // 1 second delay
    }

    ESP_ERROR_CHECK(adc_cali_delete_scheme_curve_fitting(cali_handle));
}
