/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of changing brightness of a LED connected to pin 4 (GPIO4)
 * using PWM signals. Look at:
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/peripherals/ledc.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <driver/ledc.h>
#include <stdio.h>
#include <unistd.h>

#define SLEEP 250000
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT // Duty resolution
#define LEDC_FREQUENCY 4000U            // PWM frequency in Hertz
#define LEDC_DUTY_PERCENT_TO_VALUE(percent)                                    \
  (uint32_t)(((percent) / 100.0f) * (1 << LEDC_DUTY_RES))

void app_main(void) {
  // Prepare and then apply the LEDC PWM timer configuration
  ledc_timer_config_t ledc_timer = {
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .timer_num = LEDC_TIMER,
      .duty_resolution = LEDC_DUTY_RES,
      .freq_hz = LEDC_FREQUENCY, // Set output frequency at 4 kHz
      .clk_cfg = LEDC_AUTO_CLK};
  ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

  // Prepare and then apply the LEDC PWM channel configuration
  ledc_channel_config_t ledc_channel = {.speed_mode = LEDC_LOW_SPEED_MODE,
                                        .channel = LEDC_CHANNEL,
                                        .timer_sel = LEDC_TIMER,
                                        .intr_type = LEDC_INTR_DISABLE,
                                        .gpio_num = GPIO_NUM_4};
  ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

  while (1) {
    ESP_ERROR_CHECK(
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL,
                      LEDC_DUTY_PERCENT_TO_VALUE(0))); // Set duty to 25%
    ESP_ERROR_CHECK(
        ledc_update_duty(LEDC_LOW_SPEED_MODE,
                         LEDC_CHANNEL)); // Update duty to apply the new value

    usleep(SLEEP);

    ESP_ERROR_CHECK(
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL,
                      LEDC_DUTY_PERCENT_TO_VALUE(25))); // Set duty to 25%
    ESP_ERROR_CHECK(
        ledc_update_duty(LEDC_LOW_SPEED_MODE,
                         LEDC_CHANNEL)); // Update duty to apply the new value

    usleep(SLEEP);

    ESP_ERROR_CHECK(
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL,
                      LEDC_DUTY_PERCENT_TO_VALUE(50))); // Set duty to 50%
    ESP_ERROR_CHECK(
        ledc_update_duty(LEDC_LOW_SPEED_MODE,
                         LEDC_CHANNEL)); // Update duty to apply the new value

    usleep(SLEEP);

    ESP_ERROR_CHECK(
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL,
                      LEDC_DUTY_PERCENT_TO_VALUE(75))); // Set duty to 75%
    ESP_ERROR_CHECK(
        ledc_update_duty(LEDC_LOW_SPEED_MODE,
                         LEDC_CHANNEL)); // Update duty to apply the new value

    usleep(SLEEP);

    ESP_ERROR_CHECK(
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL,
                      LEDC_DUTY_PERCENT_TO_VALUE(100))); // Set duty to 100%
    ESP_ERROR_CHECK(
        ledc_update_duty(LEDC_LOW_SPEED_MODE,
                         LEDC_CHANNEL)); // Update duty to apply the new value

    usleep(SLEEP);
  }
}
