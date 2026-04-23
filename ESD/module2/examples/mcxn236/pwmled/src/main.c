/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of changing brightness of a LED connected to J3:15(P3_12) using pwm signals.
 *        Look at: https://docs.zephyrproject.org/latest/doxygen/html/group__pwm__interface.html
 *                 https://docs.zephyrproject.org/latest/build/dts/api/bindings/clock/pwm-clock.html
 *                 https://docs.zephyrproject.org/latest/build/dts/api/bindings/led/pwm-leds.html
 *
 * @version 0.1
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(pwmled);

#define PWM_LED_NODE DT_NODELABEL(pwm_led)
#define PWM_LED_PERIOD DT_PWMS_PERIOD(PWM_LED_NODE)

int main(void)
{
	const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(PWM_LED_NODE);

	if (!pwm_is_ready_dt(&pwm_led))
	{
		LOG_ERR("Error: didn't find %s device", DT_NODE_FULL_NAME(PWM_LED_NODE));
		return EXIT_FAILURE;
	}

	uint32_t pulse_width = 0;

	while (1)
	{
		if (0 != pwm_set_pulse_dt(&pwm_led, pulse_width))
		{
			LOG_ERR("Failed to set pulse width");
		}

		pulse_width += (PWM_LED_PERIOD / 100);

		if (pulse_width > PWM_LED_PERIOD)
		{
			pulse_width = 0;
		}

		k_sleep(K_MSEC(50));
	}

	return EXIT_SUCCESS;
}
