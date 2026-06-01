/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using a CAN controller to send and receive messages
 *        Look at: https://docs.zephyrproject.org/latest/hardware/peripherals/can/controller.html
 *                 https://docs.zephyrproject.org/latest/doxygen/html/group__can__interface.html
 *
 * @version 0.1
 * @date 2025-04-01
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <zephyr/kernel.h>
#include <zephyr/sys_clock.h>
#include <zephyr/drivers/can.h>

#define CAN_NODE DT_CHOSEN(zephyr_canbus)
#define CAN_BITRATE DT_PROP(DT_PATH(zephyr_user), can_speed)

#define TIMER_PERIOD 100
#define RTR_FRAME_ID 0x200
#define SIGNAL_FRAME_ID 0x201

static volatile bool signal = false;
static volatile bool rtr_req = false;
static struct can_frame rtr_res = {.id = RTR_FRAME_ID};
static struct can_frame signal_frame = {.id = SIGNAL_FRAME_ID}; // dlc is 0

static void can_rx_callback(const struct device *dev, struct can_frame *frame, void *user_data)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(user_data);

	if (frame->id == RTR_FRAME_ID)
	{
		rtr_res.dlc = frame->dlc;
		rtr_req = true;
	}
}

static void on_expire(struct k_timer *)
{
	signal = true;
}

int main(void)
{
	const struct device *const can_dev = DEVICE_DT_GET(CAN_NODE);

	if (!device_is_ready(can_dev))
	{
		printk("CAN device not found!\n");
		exit(EXIT_FAILURE);
	}

	(void)can_stop(can_dev);

	if (0 != can_set_mode(can_dev, CAN_MODE_3_SAMPLES))
	{
		printk("Failed to set the CAN controller in loopback mode!\n");
		exit(EXIT_FAILURE);
	}

	struct can_timing timing;

	/* Set the bitrate with the sampling point at 87.5% */
	if (0 != can_calc_timing(can_dev, &timing, CAN_BITRATE, 875))
	{
		printk("Failed to calc a valid timing");
		exit(EXIT_FAILURE);
	}

	if (0 != can_set_timing(can_dev, &timing))
	{
		printk("Failed to set timing\n");
		exit(EXIT_FAILURE);
	}

	if (0 != can_start(can_dev))
	{
		printk("Failed to start the CAN controller!\n");
		exit(EXIT_FAILURE);
	}

	const struct can_filter filter = {.id = RTR_FRAME_ID, .mask = 0x7FF};
	if (0 != can_add_rx_filter(can_dev, can_rx_callback, NULL, &filter))
	{
		printk("Failed to set the filter\n");
		exit(EXIT_FAILURE);
	}

	struct k_timer timer;

	// Initialize the timer
	k_timer_init(&timer, on_expire, NULL);

	// Start the timer to run periodically
	k_timer_start(&timer, K_MSEC(0), K_MSEC(TIMER_PERIOD));

	srand((uint32_t)k_uptime_ticks());

	while (1)
	{
		if (signal)
		{
			signal = false;

			if (0 != can_send(can_dev, &signal_frame, K_NO_WAIT, NULL, NULL))
			{
				printk("Failed to send the signal frame\n");
			}
		}

		if (rtr_req)
		{
			rtr_req = false;

			memset(rtr_res.data, 0, rtr_res.dlc);
			for (int i = 0; i < rtr_res.dlc; i++)
			{
				rtr_res.data[i] = 'A' + (rand() % 26);
			}

			if (0 != can_send(can_dev, &rtr_res, K_NO_WAIT, NULL, NULL))
			{
				printk("Failed to send the response\n");
			}
		}
	}

	return 0;
}
