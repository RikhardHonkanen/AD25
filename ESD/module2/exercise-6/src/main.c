#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <esp_task_wdt.h>

#define NUM_MIN 2
#define NUM_MAX 1000
#define FIRST_PRIME 2
#define STRINGIFY(x) #x
#define LENGTH(x) sizeof(STRINGIFY(x))

static int get_number(void)
{
    int i = 1;
    int number = 0;

    while (i < LENGTH(NUM_MAX))
    {
        int chr = getchar(); // getchar is non-blocking on the ESP32

        if ((chr >= '0') && (chr <= '9'))
        {
            i++;
            putchar(chr);
            number = 10 * number + (chr - '0');
        }
        else if (chr == '\n')
        {
            break;
        }
        else
        {
            ;
        }
    }

    return number;
}

static bool isprime(int number)
{
    bool prime = true;

    if (number < FIRST_PRIME)
    {
        prime = false;
    }
    else
    {
        if (number > FIRST_PRIME)
        {
            int counter = FIRST_PRIME;
            int max_number = ceil(sqrt(number));

            while (counter <= max_number)
            {
                if (number % counter == 0)
                {
                    prime = false;
                    break;
                }

                counter++;
            }
        }
    }

    return prime;
}

void app_main(void)
{
    // Exclude the Idle Task from the Task WDT
    //  You can also do it by running: pio run -t menuconfig
    //  Component Config -> ESP System Settings -> Watch CPU0 Idle Task
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));

    int number;

    while (1)
    {
        // Clear the input buffer
        while (EOF != getchar())
        {
            ;
        }

        printf("Enter a number in the range of [%d, %d]: ", NUM_MIN, NUM_MAX);

        number = get_number();

        putchar('\n');

        if ((number >= NUM_MIN) && (number <= NUM_MAX))
        {
            printf("%d is %sprime!\n\n", number, isprime(number) ? "" : "not ");
        }
    }
}
