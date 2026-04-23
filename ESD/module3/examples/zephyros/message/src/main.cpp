#include "random.h"
#include <iostream>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel/thread_stack.h>

constexpr int PRIORITY{5};
constexpr int STACKSIZE{512}; // Size of stack area used by each thread

K_THREAD_STACK_DEFINE(stack_area, STACKSIZE);

constexpr int PTIME{500}; // Producing base time
constexpr int CTIME{500}; // Consuming base time

class Product
{
	uint32_t id{0};
	uint32_t price{0};
	const char *name{nullptr};

public:
	Product() = default;
	Product(uint32_t _id, uint32_t _price, const char *_name) : id{_id}, price{_price}, name{_name} {}

	friend std::ostream &operator<<(std::ostream &os, const Product &product)
	{
		os << "ID: " << product.id << std::endl;
		os << "Price: " << product.price << std::endl;
		os << "Name: " << product.name << std::endl;
		return os;
	}
};

// Statically define and initialize a message queue.
// Look at: https://docs.zephyrproject.org/latest/kernel/services/data_passing/message_queues.html
K_MSGQ_DEFINE(store, sizeof(Product), 8, 1); // Max. number of messages is 8

static void consumer(void *param, void *, void *)
{
	Product product;
	uint16_t value{0};
	const uint32_t cid{(uint32_t)param};

	while (1)
	{
		if (0 != k_msgq_get(&store, &product, K_FOREVER))
		{
			printf("Failed to restore a product\n");
		}

		std::cout << "============== Consumer " << cid << " ==============" << std::endl;
		std::cout << product << std::endl;

		random_get((uint8_t *)&value, sizeof(value));
		k_msleep(CTIME + (value % CTIME));
	}
}

// Look at: https://docs.zephyrproject.org/latest/kernel/services/threads/index.html
K_THREAD_DEFINE(cid1, STACKSIZE, consumer, (void *)1, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(cid2, STACKSIZE, consumer, (void *)2, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(cid3, STACKSIZE, consumer, (void *)3, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(cid4, STACKSIZE, consumer, (void *)4, NULL, NULL, PRIORITY, 0, 0);

int main(void)
{
	if (!random_init())
	{
		std::cout << "Failed to seed the random number generator" << std::endl;
		return EXIT_FAILURE;
	}

	uint16_t value;
	Product product;
	const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);

	k_thread_start(cid1);
	k_thread_start(cid2);
	k_thread_start(cid3);
	k_thread_start(cid4);

	(void)gpio_is_ready_dt(&led);
	(void)gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	while (1)
	{
		random_get((uint8_t *)&value, sizeof(value));

		switch (value % 4)
		{
		case 0:
			product = Product{100, 1000, "iPhone 14 Pro Max"};
			break;

		case 1:
			product = Product{200, 2000, "Samsung Galaxy S23 5G"};
			break;

		case 2:
			product = Product{300, 3000, "Apple Watch S9 45mm GPS+CEL"};
			break;

		default:
			product = Product{400, 4000, "Samsung Galaxy Watch5 Pro 45mm LTE"};
			break;
		}

		if (0 != k_msgq_put(&store, &product, K_FOREVER))
		{
			std::cout << "Failed to store the product" << std::endl;
		}

		(void)gpio_pin_set_dt(&led, 1);
		k_msleep(PTIME / 4);
		(void)gpio_pin_set_dt(&led, 0);

		k_msleep(3 * PTIME / 4 + (value % PTIME));
	}

	return 0;
}