/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of setting and getting the internal RTC using the functions in posix standard time.h
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/system/system_time.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <time.h>
#include <stdio.h>
#include <esp_sntp.h>
#include <esp_task_wdt.h>

#define FORMAT "YYYY-MM-DD hh:mm:ss"

enum
{
    OKAY,
    NO_DATETIME,
    INVALID_DATETIME
};

static int read_command(void)
{
    // Clear the input buffer
    while (EOF != getchar())
    {
        ;
    }

    int chr = EOF;

    while ((chr != 'S') && (chr != 'G') && (chr != '\n'))
    {
        chr = toupper(getchar());

        if (chr == '\r')
        {
            chr = '\n';
        }
    }

    putchar(chr);

    return chr;
}

static int get_datetime(struct tm *datetime)
{
    char ch = 0;
    size_t i = 0;
    int status = OKAY;
    char string[sizeof(FORMAT)] = {0};

    while (i < (sizeof(string) - 1))
    {
        ch = getchar();
        if (isdigit(ch) || isspace(ch) || (ch == '-') || (ch == ':') || (ch == '\n'))
        {
            if (ch == '\n')
            {
                break;
            }

            putchar(ch);
            string[i] = ch;
            i++;
        }
    }

    printf("\n");

    if (i == 0)
    {
        status = NO_DATETIME;
    }
    else if (i < sizeof(FORMAT) - 1)
    {
        status = INVALID_DATETIME;
    }
    else
    {
        i = 0;
        while (i < sizeof(FORMAT) - 1)
        {
            if (isalpha((int)FORMAT[i]))
            {
                if (!isdigit((int)string[i]))
                {
                    status = INVALID_DATETIME;
                    break;
                }
            }
            else
            {
                if (FORMAT[i] != string[i])
                {
                    status = INVALID_DATETIME;
                    break;
                }
            }
            i++;
        }

        if (status == OKAY)
        {
            (void)sscanf(string, "%04d-%02d-%02d %02d:%02d:%02d",
                         &datetime->tm_year, &datetime->tm_mon, &datetime->tm_mday,
                         &datetime->tm_hour, &datetime->tm_min, &datetime->tm_sec);

            if ((datetime->tm_year < atoi(__DATE__ + 7)) ||
                (datetime->tm_mon < 1) || (datetime->tm_mon > 12) || (datetime->tm_mday < 1) || (datetime->tm_mday > 31) ||
                (datetime->tm_hour > 59) || (datetime->tm_min > 59) || (datetime->tm_sec > 59))
            {
                status = INVALID_DATETIME;
            }
            else
            {
                uint8_t days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

                if (datetime->tm_mday > days[datetime->tm_mon - 1])
                {
                    status = INVALID_DATETIME;

                    if (datetime->tm_mon == 2)
                    {
                        if ((datetime->tm_year % 400 == 0) || ((datetime->tm_year % 4 == 0) && (datetime->tm_year % 100 != 0))) // If the year is a leap year
                        {
                            if (datetime->tm_mday == days[datetime->tm_mon - 1] + 1) // February we can have 29 days
                            {
                                status = OKAY;
                            }
                        }
                    }
                }

                datetime->tm_mon -= 1;
                datetime->tm_year -= 1900;
            }
        }
    }

    return status;
}

void app_main(void)
{
    // Exclude the Idle Task from the Task WDT
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));

    while (1)
    {
        printf("S) Set date and time\n"
               "G) Get date and time\n"
               "Enter a command: ");

        int command = read_command();

        switch (command)
        {
        case 'S':
        {
            struct tm datetime = {0};
            printf("\nEnter the current date and time (%s): ", FORMAT);

            switch (get_datetime(&datetime))
            {
            case OKAY:
            {
                struct timeval now = {0};
                now.tv_sec = mktime(&datetime);
                settimeofday(&now, NULL);
            }
            break;
            case INVALID_DATETIME:
                printf("Invalid date and time!\n");
                break;
            default:
                break;
            }

            printf("\n");
        }
        break;

        case 'G':
        {
            time_t now = time(NULL);
            char datetime[32] = {0};
            struct tm *timeinfo = localtime(&now);

            // For details of strftime look at: https://en.cppreference.com/w/c/chrono/strftime
            strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", timeinfo);

            printf("\nThe current datetime: %s\n\n", datetime);
        }
        break;

        case '\n':
            printf("\n");
            break;
        default:
            break;
        }
    }
}
