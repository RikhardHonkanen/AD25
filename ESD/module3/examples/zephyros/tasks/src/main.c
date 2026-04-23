#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/kernel/thread_stack.h>

#define TIMER_PERIOD_MS 500		   // Timer interval in milliseconds
#define TIME_INITIAL_DURATION 3000 // Delay before the timer gets started

#define PRIORITY 5	  // Scheduling priority used by each thread
#define STACKSIZE 512 // Size of stack area used by each thread

K_SEM_DEFINE(semaphore, 0, K_SEM_MAX_LIMIT);
K_THREAD_STACK_DEFINE(stack_area, STACKSIZE);

const struct gpio_dt_spec white_led = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), led_gpios); // The LED connected to pin 25

// Deferred interrupt handling using a semaphore
static void btn_isr(const struct device *, struct gpio_callback *, gpio_port_pins_t)
{
	k_sem_give(&semaphore);
}

static void pressed(void *, void *, void *)
{
	size_t count = 0;
	struct gpio_callback button_callback;
	const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), btn_gpios);

	__ASSERT(gpio_is_ready_dt(&button), "GPIO device is not ready!");
	__ASSERT(0 == gpio_pin_configure_dt(&button, GPIO_INPUT), "Failed to configure the gpio pin");
	__ASSERT(0 == gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE), "Failed to configure the gpio pin interrupt");
	gpio_init_callback(&button_callback, btn_isr, BIT(button.pin));
	__ASSERT(0 == gpio_add_callback(button.port, &button_callback), "Failed to add the gpio callback");

	while (1)
	{
		if (0 == k_sem_take(&semaphore, K_FOREVER))
		{
			count++;
			printk("Count = %d\n", count);
		}
	}
}

void on_expire(struct k_timer *)
{
	__ASSERT(0 == gpio_pin_toggle_dt(&white_led), "Falied to toggle the LED");
}

int main(void)
{
	struct k_thread thread;
	(void)k_thread_create(&thread, stack_area, K_THREAD_STACK_SIZEOF(stack_area), pressed, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);

	__ASSERT(gpio_is_ready_dt(&white_led), "GPIO port is not ready!");
	__ASSERT(0 == gpio_pin_configure_dt(&white_led, GPIO_OUTPUT_INACTIVE), "Failed to configure the gpio pin");

	struct k_timer timer;
	k_timer_init(&timer, on_expire, NULL);										   // Initialize the timer
	k_timer_start(&timer, K_MSEC(TIME_INITIAL_DURATION), K_MSEC(TIMER_PERIOD_MS)); // Start the timer to run periodically

	const struct gpio_dt_spec red_led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios); // The builtin red LED
	__ASSERT(gpio_is_ready_dt(&red_led), "GPIO port is not ready!");
	__ASSERT(0 == gpio_pin_configure_dt(&red_led, GPIO_OUTPUT_INACTIVE), "Failed to configure the gpio pin");

	while (1)
	{
		__ASSERT(0 == gpio_pin_toggle_dt(&red_led), "Falied to toggle the red LED");

		k_msleep(500); // 0.5s delay
	}

	return 0;
}
