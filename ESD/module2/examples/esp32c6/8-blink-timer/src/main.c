/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of making a LED connected to pin 4 (GPIO4) blinking every 0.5s using a general purpose timer (gptimer)
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/peripherals/gptimer.html
 *                 https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/peripherals/gpio.html
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
#include <esp_attr.h>
#include <driver/gpio.h>
#include <driver/gptimer.h>

#define INTERVAL 500000 // 0.5s interval

static const char *TAG = "Example";

static bool timer_on_alarm(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    (void)edata;
    (void)timer;
    (void)user_data;

    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_4, !gpio_get_level(GPIO_NUM_4)));

    // return whether we need to yield at the end of ISR
    return true; // Whether a high priority task will be awakened by this function
}

void app_main(void)
{
    ESP_LOGI(TAG, "Create timer handle");
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    ESP_LOGI(TAG, "Register the event handler");
    gptimer_event_callbacks_t cbs = {
        .on_alarm = timer_on_alarm,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));

    ESP_LOGI(TAG, "Enable timer");
    ESP_ERROR_CHECK(gptimer_enable(gptimer));

    ESP_LOGI(TAG, "Start timer at alarm event");
    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = INTERVAL, // period = 0.5s
        .flags.auto_reload_on_alarm = 1,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
    ESP_ERROR_CHECK(gptimer_start(gptimer));

    ESP_LOGI(TAG, "Configure pin 4 as a digital IO");
    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_4));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT_OUTPUT));

    while (1)
    {
        printf("Hello World!\n");
        sleep(1); // 1 second delay
    }
}
