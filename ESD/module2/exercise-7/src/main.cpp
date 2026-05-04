#include <stdio.h>
#include <esp_timer.h>
#include <driver/gpio.h>
#include <esp_task_wdt.h>

class Metro
{
    int64_t prev_micros;
    const int64_t interval;

public:
    Metro(int64_t _interval) : prev_micros{esp_timer_get_time()}, interval{1000 * _interval} {}

    bool check(void)
    {
        bool checked{false};
        int64_t now{esp_timer_get_time()};

        if (now - prev_micros >= interval)
        {
            checked = true;
            prev_micros = now;
        }

        return checked;
    }
};

extern "C" void app_main(void)
{
    // Exclude the Idle Task from the Task WDT
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));

    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_4));

    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT_OUTPUT)); // Configure pin 4 as a digital IO

    Metro blink{500};
    Metro hello{1000};

    while (1)
    {
        if (blink.check())
        {
            ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_4, !gpio_get_level(GPIO_NUM_4)));
        }

        if (hello.check())
        {
            printf("Hello World\n");
        }
    }
}
