/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of exchanging messages using a FreeRTOS Queue.
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
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

constexpr int STORE_CAPACITY{8};
constexpr int PRODUCING_TIME{500};
constexpr int CONSUMING_TIME{100};
constexpr int TASK_STACK_SIZE{256};
constexpr int CONSUMER_PRIORITY{tskIDLE_PRIORITY + 1};
constexpr int PRODUCER_PRIORITY{tskIDLE_PRIORITY + 2};

struct Product {
	uint32_t price;
	const char *name;
};

static QueueHandle_t store{nullptr};

static void producer(void *param)
{
    (void)param;

    while (1)
    {
        Product product{17000U, "iPhone 17 Max Pro"};

        if (pdPASS != xQueueSend(store, &product, portMAX_DELAY))
        {
            printf("Failed to produce a product\r\n");
        }

        vTaskDelay(pdMS_TO_TICKS(PRODUCING_TIME));
    }
}

static void consumer(void *param)
{
    (void)param;

    Product product;

    while (1)
    {
        if (pdPASS == xQueueReceive(store, &product, portMAX_DELAY))
        {
        	printf("Product: {Price: %lu SEK, Name: %s}\r\n", product.price, product.name);
        }

        vTaskDelay(pdMS_TO_TICKS(CONSUMING_TIME));
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

    // Create the queue
    store = xQueueCreate(STORE_CAPACITY, sizeof(Product));
    if (store == nullptr)
    {
        print_error("Failed to create the store");
    }

    // Create the producer task
    if (pdPASS != xTaskCreate(producer, "Producer", TASK_STACK_SIZE, nullptr, PRODUCER_PRIORITY, nullptr))
    {
        print_error("Failed to create the producer task");
    }

    // Create the consumer task
    if (pdPASS != xTaskCreate(consumer, "Consumer", TASK_STACK_SIZE, nullptr, CONSUMER_PRIORITY, nullptr))
    {
        print_error("Failed to create the consumer task");
    }

    vTaskStartScheduler();

    // If all is well, the scheduler is now running, and the following line will never be reached.
    print_error("Failed to start the scheduler.\r\n");

    return 0;
}
