#include <stdio.h>
#include <esp_timer.h>
#include <driver/gpio.h>
#include <esp_task_wdt.h>

#define UNCERTAINTY_TIME 250000 // 250ms

static volatile bool pressed = false;

// Define the GPIO interrupt handler
static void gpio_isr_handler(void *arg)
{
    (void)arg; // To avoid the warning of unused variable
    static int64_t previous = 0;

    ESP_ERROR_CHECK(gpio_intr_disable(GPIO_NUM_4)); // Disable the GPIO interrupt

    int64_t now = esp_timer_get_time();
    if (now - previous >= UNCERTAINTY_TIME)
    {
        previous = now;
        pressed = true;
    }

    ESP_ERROR_CHECK(gpio_intr_enable(GPIO_NUM_4)); // Enable the GPIO interrupt
}

void app_main(void)
{
    // Exclude the Idle Task from the Task WDT
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));

    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_4));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(GPIO_NUM_4, GPIO_PULLUP_ONLY));
    ESP_ERROR_CHECK(gpio_set_intr_type(GPIO_NUM_4, GPIO_INTR_NEGEDGE));

    ESP_ERROR_CHECK(gpio_install_isr_service(0)); // Install the GPIO ISR handler service

    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_NUM_4, gpio_isr_handler, NULL)); // Add the ISR handler

    while (1)
    {
        if (pressed)
        {
            ESP_ERROR_CHECK(gpio_intr_disable(GPIO_NUM_4)); // Disable the GPIO interrupt
            pressed = false;
            ESP_ERROR_CHECK(gpio_intr_enable(GPIO_NUM_4)); // Enable the GPIO interrupt

            printf("Falling Edge\n");
        }
    }

    ESP_ERROR_CHECK(gpio_isr_handler_remove(GPIO_NUM_4));
    gpio_uninstall_isr_service();
}
