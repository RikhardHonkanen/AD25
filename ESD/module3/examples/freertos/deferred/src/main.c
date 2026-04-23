/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of deferred interrupt handling, task creation and synchronization.
 *        Connect your push button to pin 5 (GPIO5) and your LED to pin 4 (GPIO4)
 *        Look at: https://www.freertos.org/Documentation/00-Overview
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>

#define INTERVAL 500U
#define STACK_SIZE 2048U

static TaskHandle_t press_task;
static SemaphoreHandle_t hello_sem; // Used by the FreeRTOS tick hook function and the hello task

// The RTOS tick hook function
// To enable this function, set configUSE_TICK_HOOK in FreeRTOSConfig_Gen.h
void vApplicationTickHook(void)
{
    static uint32_t tick_count = 0;

    tick_count++;

    // Give the hello semaphore on every 100th tick interrupt
    if (tick_count >= 200UL)
    {
        tick_count = 0UL;

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        // This function is called from an interrupt context (the RTOS tick interrupt),
        //  so only ISR safe API functions can be used (those that end in "FromISR()".
        xSemaphoreGiveFromISR(hello_sem, &xHigherPriorityTaskWoken);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

static void hello_task(void *param)
{
    (void)param;

    while (1)
    {
        // Block until the semaphore is 'given'
        if (pdTRUE != xSemaphoreTake(hello_sem, portMAX_DELAY))
        {
            printf("Failed to take semaphore.\r\n");
        }

        printf("Hello world!\n");
    }
}

// Deferred interrupt handling
// Look at: https://www.freertos.org/deferred_interrupt_processing.html
static void button_isr(void *param)
{
    (void)param;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(press_task, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void pressed(void *param)
{
    (void)param;

    printf("Initialize the %s ...\n", pcTaskGetName(NULL));

    // Configure pin 5 as a digital input pin for the button
    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_5));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_5, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(GPIO_NUM_5, GPIO_PULLUP_ONLY));
    ESP_ERROR_CHECK(gpio_set_intr_type(GPIO_NUM_5, GPIO_INTR_NEGEDGE));

    ESP_ERROR_CHECK(gpio_install_isr_service(0)); // Install the GPIO ISR handler service

    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_NUM_5, button_isr, NULL)); // Add the ISR handler

    // Configure pin 4 as a digital input/output pin
    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_4));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT_OUTPUT));

    while (1)
    {
        // Look at: https://www.freertos.org/ulTaskNotifyTake.html
        if (0 < ulTaskNotifyTake(pdFALSE, portMAX_DELAY))
        {
            // Toggle the LED connected to pin 5.
            ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_4, !gpio_get_level(GPIO_NUM_4)));
            for (volatile int i = 0; i < 0xFFFFF; i++)
            {
                ;
            }
        }
    }
}

static void print_error(const char *msg)
{
    while (1)
    {
        printf("%s\n", msg);
        vTaskDelay(pdMS_TO_TICKS(2 * INTERVAL));
    }
}

void app_main(void)
{
    // Initialize the binary semaphore
    hello_sem = xSemaphoreCreateCounting(1, 0); // xSemaphoreCreateMutex() or xSemaphoreCreateBinary()

    if (hello_sem == NULL)
    {
        print_error("Failed to create the semaphores!");
    }

    // Create the hello task at priority one
    if (pdPASS != xTaskCreate(hello_task, "hello_task", configMINIMAL_STACK_SIZE, NULL, 1, NULL))
    {
        print_error("Failed to create the hello task!");
    }

    // Create the pressed task at priority two
    if (pdPASS != xTaskCreate(pressed, "pressed", STACK_SIZE, NULL, 2, &press_task))
    {
        print_error("Failed to create the pressed task!");
    }
}
