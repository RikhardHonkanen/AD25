#include <ctype.h>
#include <stdlib.h>
#include <zephyr/console/console.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/kernel.h>

#define FORMAT "YYYY-MM-DD hh:mm:ss"

enum
{
    OKAY,
    NO_DATETIME,
    INVALID_DATETIME
};

static int read_command(void)
{
    int chr = EOF;

    while ((chr != 'S') && (chr != 'G') && (chr != '\n'))
    {
        chr = toupper(console_getchar());

        if (chr == '\r')
        {
            chr = '\n';
        }
    }

    console_putchar(chr);

    return chr;
}

static int get_datetime(struct rtc_time *datetime)
{
    char ch = 0;
    size_t i = 0;
    int status = OKAY;
    char string[sizeof(FORMAT)] = {0};

    while (i < (sizeof(string) - 1))
    {
        ch = console_getchar();
        if (isdigit(ch) || isspace(ch) || (ch == '-') || (ch == ':') || (ch == '\r') ||
            (ch == '\n'))
        {
            if ((ch == '\r') || (ch == '\n'))
            {
                break;
            }

            console_putchar(ch);
            string[i] = ch;
            i++;
        }
    }

    printk("\r\n");

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
            if (isalpha(FORMAT[i]))
            {
                if (!isdigit(string[i]))
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
            (void)sscanf(string, "%04d-%02d-%02d %02d:%02d:%02d", &datetime->tm_year,
                         &datetime->tm_mon, &datetime->tm_mday, &datetime->tm_hour,
                         &datetime->tm_min, &datetime->tm_sec);

            if ((datetime->tm_year < atoi(__DATE__ + 7)) || (datetime->tm_mon < 1) ||
                (datetime->tm_mon > 12) || (datetime->tm_mday < 1) || (datetime->tm_mday > 31) ||
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
                        if ((datetime->tm_year % 400 == 0) ||
                            ((datetime->tm_year % 4 == 0) &&
                             (datetime->tm_year % 100 != 0))) // If the year is a leap year
                        {
                            if (datetime->tm_mday ==
                                days[datetime->tm_mon - 1] + 1) // February we can have 29 days
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

int main(void)
{
    static const struct device *rtc_dev = DEVICE_DT_GET(DT_ALIAS(rtc));

    if (!device_is_ready(rtc_dev))
    {
        printk("The RTC device is not ready!\r\n");
        exit(EXIT_FAILURE);
    }

    if (0 != console_init())
    {
        printk("Failed to initialize console!\r\n");
        exit(EXIT_FAILURE);
    }

    struct rtc_time datetime = {0};

    while (1)
    {
        printk("S) Set date and time\r\n"
               "G) Get date and time\r\n"
               "Enter a command: ");

        int command = read_command();

        switch (command)
        {
        case 'S':
        {
            printk("\r\nEnter the current date and time (%s): ", FORMAT);

            switch (get_datetime(&datetime))
            {
            case OKAY:
            {
                if (0 != rtc_set_time(rtc_dev, &datetime))
                {
                    printk("Failed to set the RTC time!\r\n");
                    exit(EXIT_FAILURE);
                }
            }
            break;
            case INVALID_DATETIME:
                printk("Invalid date and time!\r\n");
                break;
            default:
                break;
            }

            printk("\r\n");
        }
        break;

        case 'G':
        {
            if (0 != rtc_get_time(rtc_dev, &datetime))
            {
                printk("Failed to get the RTC time!\r\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                printk("\r\n%04d-%02d-%02d %02d:%02d:%02d\r\n\r\n", datetime.tm_year + 1900,
                       datetime.tm_mon + 1, datetime.tm_mday, datetime.tm_hour, datetime.tm_min,
                       datetime.tm_sec);
            }
        }
        break;

        case '\n':
            printk("\r\n");
            break;
        default:
            break;
        }
    }

    return EXIT_SUCCESS;
}
