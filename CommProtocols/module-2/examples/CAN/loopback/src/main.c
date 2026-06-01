/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using a CAN controller in loopback mode.
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
#include <zephyr/drivers/can.h>

#define DLC 8
#define MSGS 3
#define CAN_NODE DT_CHOSEN(zephyr_canbus)
#define CAN_BITRATE DT_PROP(DT_PATH(zephyr_user), can_speed)

static void can_tx_callback(const struct device *dev, int error, void *user_data)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(user_data);

	if (error != 0)
	{
		printk("%d) Failed to send\n", error);
	}
}

static void can_rx_callback(const struct device *dev, struct can_frame *frame, void *user_data)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(user_data);

	printk("[0x%03X] %s\n", frame->id, frame->data);
}

int main(void)
{
	struct can_timing timing;
	const struct device *const can_dev = DEVICE_DT_GET(CAN_NODE);

	if (!device_is_ready(can_dev))
	{
		printk("CAN device not found!\n");
		exit(EXIT_FAILURE);
	}

	(void)can_stop(can_dev);

	if (0 != can_set_mode(can_dev, CAN_MODE_LOOPBACK | CAN_MODE_3_SAMPLES))
	{
		printk("Failed to set the CAN controller in loopback mode!\n");
		exit(EXIT_FAILURE);
	}

	/* Set the bitrate with the sampling point at 87.5% */
	if (0 != can_calc_timing(can_dev, &timing, CAN_BITRATE, 875))
	{
		printk("Failed to calc a valid timing\n");
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

	struct can_frame frames[MSGS] = {
		{.id = 0x100, .data = "AAAAAAA", .dlc = DLC},
		{.id = 0x101, .data = "BBBBBBB", .dlc = DLC},
		{.id = 0x111, .data = "CCCCCCC", .dlc = DLC},
	};

	// 0x101 = 001 0000 0001
	// 0x111 = 001 0001 0001
	// ---------------------
	// Mask  = 111 1110 1111

	// 0x101 = 001 0000 0001
	// 0x111 = 001 0001 0001
	// ----------------------
	//    id = 001 0000 0001

	const struct can_filter filter = {.id = 0x101, .mask = 0x7EF};
	if (0 != can_add_rx_filter(can_dev, can_rx_callback, NULL, &filter))
	{
		printk("Failed to set the filter\n");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		printk("\n");

		for (int i = 0; i < MSGS; i++)
		{
			if (0 != can_send(can_dev, &frames[i], K_NO_WAIT, can_tx_callback, NULL))
			{
				printk("Failed to send frames[%d]\n", i);
			}
		}

		k_msleep(1000);
	}

	return 0;
}
