/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of an interrupt attached to a bouncing push button connected to J2:17(P0_24).
 *        Look at: https://docs.zephyrproject.org/latest/kernel/services/interrupts.html
 *                 https://docs.zephyrproject.org/latest/samples/basic/button/README.html
 *                 https://docs.zephyrproject.org/latest/doxygen/html/group__gpio__interface.html
 *                 https://docs.zephyrproject.org/latest/doxygen/html/group__isr__apis.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/__assert.h>

static volatile uint32_t count = 0; // count shall be volatile as it is being accessed in the interrupt handler

static void buuton_handler(const struct device *port, struct gpio_callback *cb, gpio_port_pins_t pins)
{
	ARG_UNUSED(port);
	ARG_UNUSED(pins);
	ARG_UNUSED(cb);

	int key = irq_lock(); // Disable interrupts
	count++;			  // Change the shared variable
	irq_unlock(key);	  // Enable interrupts
}

int main(void)
{
	const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), button_gpios);

	__ASSERT(gpio_is_ready_dt(&button), "GPIO device is not ready!");

	__ASSERT(0 == gpio_pin_configure_dt(&button, GPIO_INPUT), "Failed to configure the gpio pin");

	__ASSERT(0 == gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE), "Failed to configure the gpio pin interrupt");

	struct gpio_callback button_callback;

	gpio_init_callback(&button_callback, buuton_handler, BIT(button.pin));

	__ASSERT(0 == gpio_add_callback(button.port, &button_callback), "Failed to add the gpio callback");

	uint32_t prev_count = count;

	while (1)
	{
		if (prev_count != count)
		{
			__ASSERT_NO_MSG(0 == gpio_pin_interrupt_configure_dt(&button, GPIO_INT_DISABLE));		 // Disbale interrupt
			count %= 100;																			 // Change the shared variable
			__ASSERT_NO_MSG(0 == gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE)); // Enable interrupt

			printk("Count = %d\n", count);
			prev_count = count;
		}
	}

	return 0;
}
