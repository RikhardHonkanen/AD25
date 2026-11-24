/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using FreeRTOS vTaskDelayUntil and Timer to run time-critical tasks.
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
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

// FreeRTOS kernel includes
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"

#define INTERVAL 500U

static TimerHandle_t xAutoReloadTimer, xOneShotTimer;

// The timers callback function
static void timer_callback(TimerHandle_t xTimer)
{
    static int count = 0;
    const int timer_id = (const int)pvTimerGetTimerID(xTimer);

    if (timer_id == 0)
    {
        printf("-- OneShotTimer was expired! --\r\n");
    }
    else
    {
        count++;
        printf("%d) AutoReloadTimer was expired!\r\n", count);
    }
}

// The task function to toggle the LED every 500ms.
// Instead of vTaskDelay, we have to use vTaskDelayUntil.
static void toggle_task(void *pvParameters)
{
    (void)pvParameters;

    printf("Initialize the %s...\r\n", pcTaskGetName(NULL));

    const port_pin_config_t port_pin_config = {
        .pullSelect = kPORT_PullDisable,                   /* Internal pull-up/down resistor is disabled */
        .pullValueSelect = kPORT_LowPullResistor,          /* Low internal pull resistor value is selected. */
        .slewRate = kPORT_FastSlewRate,                    /* Fast slew rate is configured */
        .passiveFilterEnable = kPORT_PassiveFilterDisable, /* Passive input filter is disabled */
        .openDrainEnable = kPORT_OpenDrainDisable,         /* Open drain output is disabled */
        .driveStrength = kPORT_LowDriveStrength,           /* Low drive strength is configured */
        .mux = kPORT_MuxAsGpio,                            /* Pin is configured as GPIO */
        .inputBuffer = kPORT_InputBufferEnable,            /* Digital input buffer enabled */
        .invertInput = kPORT_InputNormal,                  /* Digital input is not inverted */
        .lockRegister = kPORT_UnlockRegister,              /* Pin Control Register is not locked */
    };

    /* Configure the pin as a GPIO pin */
    CLOCK_EnableClock(kCLOCK_Port4); // Enables clock for PORT4
    PORT_SetPinConfig(PORT4, BOARD_LED_GREEN_GPIO_PIN, &port_pin_config);

    const gpio_pin_config_t gpio_pin_config = {kGPIO_DigitalOutput, 1};

    /* Config the pin as a digital output pin */
    CLOCK_EnableClock(kCLOCK_Gpio4); // Enables clock for GPIO4
    GPIO_PinInit(GPIO4, BOARD_LED_GREEN_GPIO_PIN, &gpio_pin_config);

    // Initialize xLastWakeTime with the current number of the system ticks.
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        GPIO_PortToggle(GPIO4, 1 << BOARD_LED_GREEN_GPIO_PIN);

        // Wait for the next cycle.
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(INTERVAL));
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

    xOneShotTimer = xTimerCreate("OneShot", pdMS_TO_TICKS(10 * INTERVAL), pdFALSE, (void *)0, timer_callback);
    if (xOneShotTimer != NULL)
    {
        xTimerStart(xOneShotTimer, 0);
    }
    else
    {
        print_error("Failed to create the OneShot timer!");
    }

    xAutoReloadTimer = xTimerCreate("Reload", pdMS_TO_TICKS(2 * INTERVAL), pdTRUE, (void *)1, timer_callback);
    if (xAutoReloadTimer != NULL)
    {
        xTimerStart(xAutoReloadTimer, 0);
    }
    else
    {
        print_error("Failed to create the AutoReload timer!");
    }

    if (pdPASS != xTaskCreate(toggle_task, "toggle_task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL))
    {
        print_error("Failed to create the toggle task!");
    }

    vTaskStartScheduler();

    // If all is well, the scheduler is now running, and the following line will never be reached.
    print_error("Failed to start the scheduler.\r\n");

    return 0;
}
