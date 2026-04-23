#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel/thread_stack.h>

#define PONG BIT(0)
#define BLINK BIT(1)

#define DELAY 500
#define PRIORITY 5	  // Scheduling priority used by each thread
#define STACKSIZE 512 // Size of stack area used by each thread

K_EVENT_DEFINE(event); /* Statically define and initialize an event. I.e. struct k_event event; k_event_init(&event);
						  Look at: https://docs.zephyrproject.org/latest/kernel/services/synchronization/events.html */

static void print_ping(void)
{
	while (1)
	{
		k_msleep(DELAY);

		printk("Ping - ");

		(void)k_event_post(&event, PONG | BLINK);
	}
}

static void print_pong(void)
{
	while (1)
	{
		(void)k_event_wait(&event, PONG, true, K_FOREVER);

		printk("Pong\n");
	}
}

static void blink(void)
{
	const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

	(void)gpio_is_ready_dt(&led);

	(void)gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	while (1)
	{
		(void)k_event_wait(&event, BLINK, true, K_FOREVER);

		(void)gpio_pin_toggle_dt(&led);
	}
}

// Look at: https://docs.zephyrproject.org/latest/kernel/services/threads/index.html
K_THREAD_DEFINE(blink_id, STACKSIZE, blink, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(ping_id, STACKSIZE, print_ping, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(pong_id, STACKSIZE, print_pong, NULL, NULL, NULL, PRIORITY, 0, 0);