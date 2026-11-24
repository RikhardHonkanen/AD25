/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of creating static and dynamic FreeRTOS tasks.
 *        In FreeRTOSConfig_Gen.h, set configSUPPORT_STATIC_ALLOCATION to enable creating tasks statically.
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
#include "task.h"

#define HELLO_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define HELLO_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define TOGGLE_TASK_PRIO (configMAX_PRIORITIES - 1)
#define TOGGLE_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

// Statically allocated memory for the toggle_task task stack and control block
static StackType_t toggleTaskStack[TOGGLE_TASK_STACK_SIZE];
static StaticTask_t toggleTaskTCB;

static void hello_task(void *pvParameters)
{
    (void)pvParameters;

    while (1)
    {
        printf("\r\nHello world.");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

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

    while (1)
    {
        GPIO_PortToggle(GPIO4, 1 << BOARD_LED_GREEN_GPIO_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));
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

    if (pdPASS != xTaskCreate(hello_task, "hello_task", HELLO_TASK_STACK_SIZE, NULL, HELLO_TASK_PRIO, NULL))
    {
    	print_error("Failed to create the hello task!");
    }

    if (NULL == xTaskCreateStatic(toggle_task, "toggle_task", TOGGLE_TASK_STACK_SIZE, NULL, TOGGLE_TASK_PRIO, toggleTaskStack, &toggleTaskTCB))
    {
    	print_error("Failed to create the toggle task!");
    }

    vTaskStartScheduler();

    // If all is well, the scheduler will now be running, and the following line will never
    // be reached. If the following line does execute, then there was insufficient
    // FreeRTOS heap memory available for the idle and/or timer tasks to be created
    print_error("Failed to start the scheduler.\r\n");

    return 0;
}

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   configSTACK_DEPTH_TYPE *puxIdleTaskStackSize)
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    *puxIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    configSTACK_DEPTH_TYPE *puxTimerTaskStackSize)
{
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    *puxTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
