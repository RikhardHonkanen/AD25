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
#include <zephyr/drivers/led.h>
#include <zephyr/drivers/can.h>

#define CAN_NODE DT_CHOSEN(zephyr_canbus)
#define CAN_BITRATE DT_PROP(DT_PATH(zephyr_user), can_speed)

#define RTR_FRAME_ID 0x200
#define SIGNAL_FRAME_ID 0x201 // dlc is 0

const struct led_dt_spec green_led = LED_DT_SPEC_GET(DT_NODELABEL(green_led));

static void can_rx_callback(const struct device *dev, struct can_frame *frame, void *user_data)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(user_data);

	if (frame->id == SIGNAL_FRAME_ID)
	{
		static bool state = true;
		if (0 != (state ? led_on_dt(&green_led) : led_off_dt(&green_led)))
		{
			printk("Failed to toggle the LED\n");
		}
		state = !state;
	}
	else
	{
		printk("[0x%03X] %.*s\n", frame->id, frame->dlc, frame->data);
	}
}

int main(void)
{
	if (!led_is_ready_dt(&green_led))
	{
		printk("LED device is not ready\n");
		exit(EXIT_FAILURE);
	}

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

	struct can_frame rtr_req = {.id = RTR_FRAME_ID, .flags = CAN_FRAME_RTR};

	// 0x200 = 010 0000 0000 => RTR frame
	// 0x201 = 010 0000 0001 => Alive Signal frame
	// ---------------------
	// Mask  = 111 1111 1110

	// 0x200 = 010 0000 0000
	// 0x201 = 010 0000 0001
	// ----------------------
	//    id = 010 0000 0000

	const struct can_filter filter = {.id = 0x200, .mask = 0x7FE};
	if (0 != can_add_rx_filter(can_dev, can_rx_callback, NULL, &filter))
	{
		printk("Failed to set the filter\n");
		exit(EXIT_FAILURE);
	}

	srand((uint32_t)k_uptime_ticks());

	while (1)
	{
		rtr_req.dlc = CAN_MAX_DLC / (1 + (rand() % 2));

		if (0 != can_send(can_dev, &rtr_req, K_NO_WAIT, NULL, NULL))
		{
			printk("Failed to send the RTR frame\n");
		}

		k_msleep(1000);
	}

	return 0;
}
