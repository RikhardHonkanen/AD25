/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of setting and getting datetime of the internal RTC.
 *        Look at: https://docs.zephyrproject.org/latest/hardware/peripherals/rtc.html
 *                 https://docs.zephyrproject.org/latest/doxygen/html/group__rtc__interface.html
 *
 * @version 0.1
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/rtc.h>

int main(void)
{
	static const struct device *rtc_dev = DEVICE_DT_GET(DT_ALIAS(rtc));

	if (!device_is_ready(rtc_dev))
	{
		printk("The RTC device is not ready!\n");
		exit(1);
	}

	struct rtc_time datetime = {
		.tm_sec = 0,
		.tm_min = 0,
		.tm_hour = 10,
		.tm_mday = 1,
		.tm_mon = 4 - 1,
		.tm_year = 2025 - 1900,
	};

	if (0 != rtc_set_time(rtc_dev, &datetime))
	{
		printk("Failed to set the RTC time!\n");
		exit(1);
	}

	while (1)
	{
		if (0 != rtc_get_time(rtc_dev, &datetime))
		{
			printk("Failed to get the RTC time!\n");
			exit(1);
		}
		else
		{
			printk("%04d-%02d-%02d %02d:%02d:%02d\n",
				   datetime.tm_year + 1900, datetime.tm_mon + 1, datetime.tm_mday,
				   datetime.tm_hour, datetime.tm_min, datetime.tm_sec);
		}

		k_msleep(1000);
	}

	return 0;
}
