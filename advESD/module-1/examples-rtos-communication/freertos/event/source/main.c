/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using a FreeRTOS EventGroupHandle_t to synchronize tasks.
 *        Ensure that you add the following components to your project:
 *            1) Middleware -> RTOS -> cm33_non_trustzone_port
 *            2) Operating Systems -> RTOS -> FreeRTOS kernel
 *            3) Project Template -> RTOS -> freertos template
 *            4) Others -> RTOS -> Heap -> FreeRTOS heap 4
 *               Look at: https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/09-Memory-management/01-Memory-management
 *
 * @version 0.1
 * @date 2025-10-18
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

// FreeRTOS kernel includes
#include "event_groups.h"
#include "FreeRTOS.h"
#include "task.h"

#define BIT(n) (1 << n)
#define PING BIT(0)
#define PONG BIT(1)

static EventGroupHandle_t event_group;

// The task function to print Ping.
static void print_ping(void *param)
{
    (void)param;

    while (1)
    {
        // Sleep for 500 milliseconds.
        vTaskDelay(pdMS_TO_TICKS(500));

        (void)xEventGroupWaitBits(event_group, PING, pdTRUE, pdFALSE, portMAX_DELAY);

        printf("\r\nPing - ");

        (void)xEventGroupSetBits(event_group, PONG);
    }
}

// The task function to print Pong.
static void print_pong(void *param)
{
    (void)param;

    while (1)
    {
        (void)xEventGroupWaitBits(event_group, PONG, pdTRUE, pdFALSE, portMAX_DELAY);

        printf("Pong");

        (void)xEventGroupSetBits(event_group, PING);
    }
}

static void print_error(const char *msg)
{
    while (1)
    {
        printf("%s\r\n", msg);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();

    // Create the event group
    event_group = xEventGroupCreate();
    (void)xEventGroupSetBits(event_group, PING);

    if (pdPASS != xTaskCreate(print_ping, "print_ping", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL))
    {
        print_error("Failed to create the print_ping task!");
    }

    if (pdPASS != xTaskCreate(print_pong, "print_pong", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL))
    {
        print_error("Failed to create the print_pong task!");
    }

    vTaskStartScheduler();

    // If all is well, the scheduler is now running, and the following line will never be reached.
    print_error("Failed to start the scheduler.\r\n");

    return 0;
}
