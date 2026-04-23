/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using ADC1_CH0 (GPIO0) to read the ambient temperature using TMP36.
 *        Please ensure that the connections are correct.
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/peripherals/adc_continuous.html
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
#include <driver/ledc.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_continuous.h>
#include <esp_adc/adc_cali_scheme.h>

#define SAMPLES 16
#define MILLIVOLT_OFFSET 500.0
#define MILLIVOLT_PER_CELSIUS 10
#define ADC_RESOLUTION ADC_BITWIDTH_12
#define ADC_ATTENUATION ADC_ATTEN_DB_12
#define BUFFER_SIZE (SAMPLES * SOC_ADC_DIGI_DATA_BYTES_PER_CONV)

void app_main(void)
{
    adc_cali_handle_t cali_handle;
    adc_continuous_handle_t adc_handle;

    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = BUFFER_SIZE,
        .conv_frame_size = BUFFER_SIZE,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &adc_handle));

    adc_digi_pattern_config_t adc_pattern = {
        .atten = ADC_ATTENUATION,
        .bit_width = ADC_RESOLUTION,
        .channel = ADC_CHANNEL_0,
        .unit = ADC_UNIT_1,
    };

    adc_continuous_config_t config = {
        .pattern_num = 1,
        .sample_freq_hz = 32000,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE2,
        .adc_pattern = &adc_pattern,
    };

    ESP_ERROR_CHECK(adc_continuous_config(adc_handle, &config));

    // ADC1 Calibration
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .chan = ADC_CHANNEL_0,
        .atten = ADC_ATTENUATION,
        .bitwidth = ADC_RESOLUTION,
    };
    ESP_ERROR_CHECK(adc_cali_create_scheme_curve_fitting(&cali_config, &cali_handle));

    ESP_ERROR_CHECK(adc_continuous_start(adc_handle));

    int millivolts;
    uint32_t length;
    esp_err_t status;
    uint8_t result[BUFFER_SIZE];

    while (1)
    {
        status = adc_continuous_read(adc_handle, result, sizeof(result), &length, 0);

        if (status == ESP_OK)
        {
            uint32_t value = 0;

            for (uint32_t i = 0; i < length; i += SOC_ADC_DIGI_RESULT_BYTES)
            {
                adc_digi_output_data_t *sample = (adc_digi_output_data_t *)&result[i];
                value += sample->val;
            }

            value /= (length / SOC_ADC_DIGI_RESULT_BYTES);
            ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle, value, &millivolts));

            printf("Temperature: %.02f °C\n", (millivolts - MILLIVOLT_OFFSET) / MILLIVOLT_PER_CELSIUS);
        }
        else if (status == ESP_ERR_INVALID_STATE)
        {
            ESP_LOGE("TMP36", "Sampling rate is faster than the task processing rate!");
        }
        else
        {
            ; // Conversion timeout
        }

        sleep(1); // 1 second delay
    }
}
