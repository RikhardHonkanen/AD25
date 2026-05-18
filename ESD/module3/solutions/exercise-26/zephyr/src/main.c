#include "cbuffer.h"
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/sys_clock.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel/thread_stack.h>

#define DELAY 100
#define PRIORITY 5
#define CONSUMERS 4
#define STACKSIZE 512

static void consumer(void *param, void *, void *)
{
	const int ID = (int)param;

	while (1)
	{
		k_msleep(DELAY * (1 + rand() % CONSUMERS));
		printk("Consumer ID: %d, Value: %d\n", ID, cbuffer_restore());
	}
}

K_THREAD_STACK_DEFINE(stack, STACKSIZE);

K_THREAD_DEFINE(cid1, STACKSIZE, consumer, (void *)1, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(cid2, STACKSIZE, consumer, (void *)2, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(cid3, STACKSIZE, consumer, (void *)3, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(cid4, STACKSIZE, consumer, (void *)4, NULL, NULL, PRIORITY, 0, 0);

int main(void)
{
	cbuffer_init();

	k_thread_start(cid1);
	k_thread_start(cid2);
	k_thread_start(cid3);
	k_thread_start(cid4);

	srand((uint32_t)k_uptime_ticks());

	int value = 0;

	while (1)
	{
		value++;
		cbuffer_store(value);
		k_msleep(DELAY * (1 + rand() % CONSUMERS));
	}

	return 0;
}
