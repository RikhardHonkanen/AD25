#include "button.h"
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

#define BUTTON_PIN GPIO_NUM_4
#define LED_PIN GPIO_NUM_5

static void button_timer_callback(TimerHandle_t xTimer)
{
    button_update_state();

    if (button_get_falling_edge())
    {
        static bool led_on = false;

        led_on = !led_on;

        gpio_set_level(LED_PIN, led_on);
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK(gpio_reset_pin(BUTTON_PIN));
    ESP_ERROR_CHECK(gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY));

    ESP_ERROR_CHECK(gpio_reset_pin(LED_PIN));
    ESP_ERROR_CHECK(gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT));

    button_init(BUTTON_PIN);

    TimerHandle_t timer = xTimerCreate("button_timer",       // Name
                                       pdMS_TO_TICKS(5),     // Period: 5 ms
                                       pdTRUE,               // Auto-reload
                                       NULL,                 // Timer ID
                                       button_timer_callback // Callback
    );

    assert(timer != NULL);
    xTimerStart(timer, 0);

    while (1)
    {
        vTaskDelay(portMAX_DELAY);
    }
}