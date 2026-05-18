#include <stdio.h>
#include <freertos/FreeRTOS.h>

#define NUM_MIN 1
#define NUM_MAX 3
#define DELAY 1000
#define STACK_SIZE 2048

static TaskHandle_t tasks[NUM_MAX - NUM_MIN + 1];

static void print_number(void *param)
{
    const int NUM = (int)param;

    while (1)
    {
        if (NUM == NUM_MIN)
        {
            vTaskDelay(pdMS_TO_TICKS(DELAY)); // Sleep for 1000 milliseconds.
        }

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (NUM == NUM_MAX)
        {
            printf("%d\r\n", NUM);
            xTaskNotifyGive(tasks[0]);
        }
        else
        {
            printf("%d - ", NUM);
            xTaskNotifyGive(tasks[NUM - NUM_MIN + 1]);
        }
    }
}

static void print_error(const char *msg)
{
    while (1)
    {
        printf("%s\n", msg);
        vTaskDelay(pdMS_TO_TICKS(DELAY));
    }
}

void app_main(void)
{
    // Create the tasks
    for (int i = NUM_MIN; i <= NUM_MAX; i++)
    {
        char str[64] = {0};
        sprintf(str, "Print %d", i);
        if (pdPASS != xTaskCreate(print_number, str, STACK_SIZE, (void *)i, 1, &tasks[i - NUM_MIN]))
        {
            sprintf(str, "Failed to create the 'print %d' task", i);
            print_error(str);
        }
    }

    xTaskNotifyGive(tasks[0]);
}
