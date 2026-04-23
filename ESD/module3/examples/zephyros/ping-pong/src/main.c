#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/kernel/thread_stack.h>

#define PING 0
#define PONG 1
#define PRIORITY 7				// Scheduling priority used by each thread
#define STACKSIZE 256			// Size of stack area used by each thread
#define LED_NODE DT_ALIAS(led0) // The devicetree node identifier for the "led0" alias

K_MUTEX_DEFINE(mutex); /* Statically define and initialize a mutex. I.e. struct k_mutex mutex; k_mutex_init(&mutex);
						  Look at: https://docs.zephyrproject.org/latest/kernel/services/synchronization/mutexes.html */

K_CONDVAR_DEFINE(condvar); /* Statically define and initialize a condition variable. I.e. struct k_condvar condvar; k_condvar_init(&condvar);
							  Look at: https://docs.zephyrproject.org/latest/kernel/services/synchronization/condvar.html */

static volatile int ping_pong = PING;

static void print_ping(void)
{
	while (1)
	{
		k_msleep(1000); // 1s delay

		k_mutex_lock(&mutex, K_FOREVER);

		while (ping_pong != PING)
		{
			k_condvar_wait(&condvar, &mutex, K_FOREVER);
		}

		printk("Ping - ");
		ping_pong = PONG;

		k_condvar_signal(&condvar);
		k_mutex_unlock(&mutex);
	}
}

static void print_pong(void)
{
	while (1)
	{
		k_mutex_lock(&mutex, K_FOREVER);

		while (ping_pong != PONG)
		{
			k_condvar_wait(&condvar, &mutex, K_FOREVER);
		}

		printk("Pong\n");
		ping_pong = PING;

		k_condvar_signal(&condvar);
		k_mutex_unlock(&mutex);
	}
}

static void blink(void)
{
	const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

	__ASSERT(gpio_is_ready_dt(&led), "GPIO port is not ready!");

	__ASSERT(0 == gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE), "Failed to configure the gpio pin");

	while (1)
	{
		__ASSERT(0 == gpio_pin_toggle_dt(&led), "Falied to toggle the LED");

		k_msleep(500); // 0.5s delay
	}
}

// Look at: https://docs.zephyrproject.org/latest/kernel/services/threads/index.html
K_THREAD_DEFINE(blink_id, STACKSIZE, blink, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(ping_id, STACKSIZE, print_ping, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(pong_id, STACKSIZE, print_pong, NULL, NULL, NULL, PRIORITY, 0, 0);