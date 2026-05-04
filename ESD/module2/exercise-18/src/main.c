#include <zephyr/kernel.h>
#include <zephyr/sys_clock.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/__assert.h>

#define UNCERTAINTY_TIME 250000 // 250ms
#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)

static volatile uint32_t count = 0;
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, led_gpios);

static void buuton_handler(const struct device *port, struct gpio_callback *cb, gpio_port_pins_t pins)
{
	(void)port;
	(void)pins;
	(void)cb;

	static uint64_t prevoius = 0;

	int key = irq_lock(); // Disable all interrupts

	uint64_t current = k_ticks_to_us_floor64(k_uptime_ticks());
	if (current - prevoius >= UNCERTAINTY_TIME)
	{
		prevoius = current;
		__ASSERT(0 == gpio_pin_toggle_dt(&led), "Falied to toggle the LED");
	}

	irq_unlock(key); // Enable all interrupts
}

int main(void)
{
	__ASSERT(gpio_is_ready_dt(&led), "LED) GPIO port is not ready!");
	__ASSERT(0 == gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE), "LED) Failed to configure the gpio pin");

	const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, btn_gpios);

	__ASSERT(gpio_is_ready_dt(&button), "BTN) GPIO port is not ready!");
	__ASSERT(0 == gpio_pin_configure_dt(&button, GPIO_INPUT), "BTN) Failed to configure the gpio pin");
	__ASSERT(0 == gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE), "Failed to configure the gpio pin interrupt");

	struct gpio_callback button_callback;
	gpio_init_callback(&button_callback, buuton_handler, BIT(button.pin));
	__ASSERT(0 == gpio_add_callback(button.port, &button_callback), "Failed to add the gpio callback");

	while (1)
	{
		k_sleep(K_FOREVER);
	}

	return 0;
}
