#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <esp_random.h>
#include <driver/ledc.h>
#include <bootloader_random.h>

#define LED_RED_PIN GPIO_NUM_4
#define LEDC_RED_CHANNEL LEDC_CHANNEL_0

#define LED_GREEN_PIN GPIO_NUM_5
#define LEDC_GREEN_CHANNEL LEDC_CHANNEL_1

#define LED_BLUE_PIN GPIO_NUM_6
#define LEDC_BLUE_CHANNEL LEDC_CHANNEL_2

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT // Duty resolution
#define LEDC_FREQUENCY 100              // PWM frequency in Hertz
#define LEDC_DUTY_PERCENT_TO_VALUE(p) (uint32_t)(((p) / 100.0f) * (1 << LEDC_DUTY_RES))

static void pwm_pins_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY, // Set the pwm signal frequency
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare the LEDC PWM channel configurations
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
    };

    // Configure the red channel
    ledc_channel.gpio_num = LED_RED_PIN;
    ledc_channel.channel = LEDC_RED_CHANNEL;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // Configure the green channel
    ledc_channel.gpio_num = LED_GREEN_PIN;
    ledc_channel.channel = LEDC_GREEN_CHANNEL;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // Configure the blue channel
    ledc_channel.gpio_num = LED_BLUE_PIN;
    ledc_channel.channel = LEDC_BLUE_CHANNEL;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

static void set_color(uint8_t red, uint8_t green, uint8_t blue)
{
    // Set an apply the new duty cycle on the red pin
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_RED_CHANNEL, LEDC_DUTY_PERCENT_TO_VALUE(red)));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_RED_CHANNEL));

    // Set an apply the new duty cycle on the green pin
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_GREEN_CHANNEL, LEDC_DUTY_PERCENT_TO_VALUE(green)));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_GREEN_CHANNEL));

    // Set an apply the new duty cycle on the blue pin
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_BLUE_CHANNEL, LEDC_DUTY_PERCENT_TO_VALUE(blue)));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_BLUE_CHANNEL));
}

void app_main(void)
{
    bootloader_random_enable();
    srand(esp_random());
    bootloader_random_disable();

    pwm_pins_init();

    uint8_t red, green, blue;

    while (1)
    {
        red = rand() % 101;
        green = rand() % 101;
        blue = rand() % 101;

        set_color(red, green, blue);

        printf("RGB = (%u%%, %u%%, %u%%)\n", red, green, blue);

        sleep(1); // 1s
    }
}
