#include <stdio.h>
#include <unistd.h>
#include <driver/ledc.h>

#define FADING_STEP 5
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT // Duty resolution
#define LEDC_FREQUENCY 100              // PWM frequency in Hertz
#define LEDC_DUTY_PERCENT_TO_VALUE(p) (uint32_t)(((p) / 100.0f) * (1 << LEDC_DUTY_RES))

void app_main(void)
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

    int step, duty = 0;

    while (1)
    {
        if (duty == 100)
        {
            step = -FADING_STEP;
        }
        else if (duty == 0)
        {
            step = FADING_STEP;
        }
        else
        {
            ;
        }

        duty += step;
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY_PERCENT_TO_VALUE(duty))); // Set duty cycle
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));                                // Apply the new duty cycle

        usleep(50000);
    }
}
