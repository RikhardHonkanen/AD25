#include <stdio.h>
#include <esp_log.h>
#include <driver/ledc.h>
#include <esp_task_wdt.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_continuous.h>
#include <esp_adc/adc_cali_scheme.h>

#define SAMPLES 16
#define ADC_RESOLUTION ADC_BITWIDTH_12
#define ADC_ATTENUATION ADC_ATTEN_DB_12
#define BUFFER_SIZE (SAMPLES * SOC_ADC_DIGI_DATA_BYTES_PER_CONV)

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_12_BIT // Duty resolution
#define LEDC_FREQUENCY 100U             // PWM frequency in Hertz

static void led_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY, // Set output frequency at 4 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = GPIO_NUM_4};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

static void pot_init(adc_continuous_handle_t *adc_handle, adc_cali_handle_t *cali_handle)
{
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = BUFFER_SIZE,
        .conv_frame_size = BUFFER_SIZE,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, adc_handle));

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

    ESP_ERROR_CHECK(adc_continuous_config(*adc_handle, &config));

    // ADC1 Calibration
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .chan = ADC_CHANNEL_0,
        .atten = ADC_ATTENUATION,
        .bitwidth = ADC_RESOLUTION,
    };
    ESP_ERROR_CHECK(adc_cali_create_scheme_curve_fitting(&cali_config, cali_handle));

    ESP_ERROR_CHECK(adc_continuous_start(*adc_handle));
}

static float map(float value, float from_min, float from_max, float to_min, float to_max)
{
    if (value > from_max)
    {
        value = from_max;
    }
    else if (value < from_min)
    {
        value = from_min;
    }
    else
    {
        ;
    }

    return ((value - from_min) * ((to_max - to_min) / (from_max - from_min)) + to_min);
}

void app_main(void)
{
    // Exclude the Idle Task from the Task WDT
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));

    led_init();

    adc_cali_handle_t cali_handle;
    adc_continuous_handle_t adc_handle;
    pot_init(&adc_handle, &cali_handle);

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

            // VREF = 3.3 v = 3300 mv
            value = (uint32_t)map(millivolts, 0, 3300, 0, 1 << LEDC_DUTY_RES);

            ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, value));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
        }
        else if (status == ESP_ERR_INVALID_STATE)
        {
            ESP_LOGE("Exercise 11", "Sampling rate is faster than the task processing rate!");
        }
        else
        {
            ; // Conversion timeout
        }
    }
}
