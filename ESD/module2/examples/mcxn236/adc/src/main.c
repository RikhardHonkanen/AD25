/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using an ADC channel(LPADC0_A3) to read temperature using TMP36 sensor
 *        Look at: https://docs.zephyrproject.org/latest/doxygen/html/group__adc__interface.html
 *                 https://docs.zephyrproject.org/latest/build/dts/api/bindings/adc/nxp%2Clpc-lpadc.html
 *
 *        Connections:
 *           TMP36 .... FRDM_MCXN236
 *             Vs --------> VDDA
 *            GND --------> GND
 *            Vout -------> J4:2(P4_6) Look at: page 78 of MCXN23x.pdf
 *
 * @version 0.1
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>

#define MILLIVOLT_OFFSET 500.0
#define MILLIVOLT_PER_CELSIUS 10

int main(void)
{
	const struct adc_dt_spec tmp_dev = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

	if (!adc_is_ready_dt(&tmp_dev) || (0 > adc_channel_setup_dt(&tmp_dev)))
	{
		printk("TMP device is not ready");
		return EXIT_FAILURE;
	}

	int16_t value;
	struct adc_sequence sequence = {
		.buffer = &value,
		.buffer_size = sizeof(value),
		.calibrate = true,
	};

	int32_t millivolts;

	while (1)
	{
		if ((0 != adc_sequence_init_dt(&tmp_dev, &sequence)) || (0 != adc_read_dt(&tmp_dev, &sequence)))
		{
			printk("TMP ADC failed to read\n");
			return EXIT_FAILURE;
		}

		millivolts = value;
		if (0 > adc_raw_to_millivolts_dt(&tmp_dev, &millivolts))
		{
			printk("TMP ADC failed to convert the raw value to millivolts\n");
			return EXIT_FAILURE;
		}

		printf("Temperature = %0.1f\n", (millivolts - MILLIVOLT_OFFSET) / MILLIVOLT_PER_CELSIUS);

		k_sleep(K_MSEC(500));
	}

	return EXIT_SUCCESS;
}
