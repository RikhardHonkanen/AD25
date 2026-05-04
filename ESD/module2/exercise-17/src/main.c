#include <zephyr/kernel.h>
#include <zephyr/drivers/led.h>
#include <zephyr/drivers/adc.h>

static float map(float value, float from_min, float from_max, float to_min, float to_max)
{
	if (value > from_max)
	{
		value = from_max;
	}
	else if (value < from_min)
	{
		value = from_min;
	}
	else
	{
		;
	}

	return ((value - from_min) * ((to_max - to_min) / (from_max - from_min)) + to_min);
}

int main(void)
{
	const struct led_dt_spec led_dev = LED_DT_SPEC_GET(DT_NODELABEL(myled));
	const struct adc_dt_spec pot_dev = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

	if (!led_is_ready_dt(&led_dev))
	{
		printk("LED device is not ready");
		exit(EXIT_FAILURE);
	}

	if (!adc_is_ready_dt(&pot_dev) || (0 > adc_channel_setup_dt(&pot_dev)))
	{
		printk("POT device is not ready");
		exit(EXIT_FAILURE);
	}

	uint16_t value;
	struct adc_sequence sequence = {
		.buffer = &value,
		.buffer_size = sizeof(value),
		.calibrate = true,
	};

	int32_t millivolts;

	while (1)
	{
		if ((0 != adc_sequence_init_dt(&pot_dev, &sequence)) || (0 != adc_read_dt(&pot_dev, &sequence)))
		{
			printk("ADC failed to read\n");
			exit(EXIT_FAILURE);
		}

		millivolts = value;
		if (0 > adc_raw_to_millivolts_dt(&pot_dev, &millivolts))
		{
			printk("ADC failed to convert the raw value to millivolts\n");
			exit(EXIT_FAILURE);
		}

		value = (int)map(millivolts, 0, pot_dev.vref_mv, 0, 100);

		if (0 != led_set_brightness_dt(&led_dev, value))
		{
			printk("Failed to set brightness of the LED\n");
			exit(EXIT_FAILURE);
		}

		k_sleep(K_NSEC(DT_PWMS_PERIOD(DT_NODELABEL(myled))));
	}

	return EXIT_SUCCESS;
}
