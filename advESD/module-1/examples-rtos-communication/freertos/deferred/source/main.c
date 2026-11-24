/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of deferred interrupt handling. Look at: https://www.freertos.org/deferred_interrupt_processing.html
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
#include "semphr.h"
#include "task.h"

#define HELLO_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define HELLO_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define BUTTON_TASK_PRIO (configMAX_PRIORITIES - 1)
#define BUTTON_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

static TaskHandle_t button_task_handle;
static SemaphoreHandle_t hello_semaphore; // Used by the FreeRTOS tick hook function and the hello task

void GPIO00_IRQHandler(void)
{
	if(0 != (GPIO_GpioGetInterruptFlags(BOARD_SW3_GPIO) & (1U << BOARD_SW3_GPIO_PIN)))
	{
		GPIO_GpioClearInterruptFlags(BOARD_SW3_GPIO, 1U << BOARD_SW3_GPIO_PIN); // Clear the interrupt flag

		portASSERT_IF_INTERRUPT_PRIORITY_INVALID();

	    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    	// This function is called from an interrupt context, so only
    	// ISR safe API functions can be used (those that end in "FromISR()".
	    vTaskNotifyGiveFromISR(button_task_handle, &xHigherPriorityTaskWoken);

	    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

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
    	xSemaphoreGiveFromISR(hello_semaphore, &xHigherPriorityTaskWoken);

    	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

static void hello_task(void *pvParameters)
{
	(void)pvParameters;

	while(1)
	{
        // Block until the semaphore is 'given'
        if (pdTRUE != xSemaphoreTake(hello_semaphore, portMAX_DELAY))
        {
            printf("Failed to take semaphore.\r\n");
        }

		printf("\r\nHello world.");
	}
}

static void button_task(void *pvParameters)
{
	(void)pvParameters;

	printf("Initialize the %s...\n", pcTaskGetName(NULL));

    /* Configure the button pin as GPIO */
    CLOCK_EnableClock(kCLOCK_Port0); // Enables port clock

    const port_pin_config_t port_pin_config = {
        .pullSelect = kPORT_PullUp,                        /* Internal pull-up resistor is enabled */
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

    PORT_SetPinConfig(PORT0, BOARD_SW3_GPIO_PIN, &port_pin_config);

    CLOCK_EnableClock(kCLOCK_Gpio0); // Enables the gpio clock

    /* Config the button pin as a digital input pin */
    const gpio_pin_config_t gpio_pin_config = {kGPIO_DigitalInput};

    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &gpio_pin_config);

    GPIO_SetPinInterruptConfig(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, kGPIO_InterruptFallingEdge);

    EnableIRQ(GPIO00_IRQn); // Enable the interrupt in the NVIC

    // Priority of the interrupt shall be higher than configMAX_SYSCALL_INTERRUPT_PRIORITY which is 32
     NVIC_SetPriority(GPIO00_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);

    printf("\r\nPress SW3 on the board\r\n");

	while(1)
	{
        // Look at: https://www.freertos.org/ulTaskNotifyTake.html
        if (0 < ulTaskNotifyTake(pdFALSE, portMAX_DELAY))
        {
            printf("\r\nSW3 pressed.");
        }
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

    // Create the binary semaphore
    hello_semaphore = xSemaphoreCreateCounting(1, 0);
    if(hello_semaphore == NULL)
    {
    	print_error("Failed to create the hello binary semaphore!");
    }

    if (pdPASS != xTaskCreate(hello_task, "hello_task", HELLO_TASK_STACK_SIZE, NULL, HELLO_TASK_PRIO, NULL))
    {
    	print_error("Failed to create the hello task!");
    }

    if (pdPASS != xTaskCreate(button_task, "button_task", BUTTON_TASK_STACK_SIZE, NULL, BUTTON_TASK_PRIO, &button_task_handle))
    {
    	print_error("Failed to create the button task!");
    }

    vTaskStartScheduler();

    // If all is well, the scheduler is now running, and the following line will never be reached.
    print_error("Failed to start the scheduler.\r\n");

    return 0 ;
}
