#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/sys_clock.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/console/console.h>

#define RED_PWM_NODE DT_NODELABEL(pwm_red)
#define RED_PWM_PERIOD DT_PWMS_PERIOD(RED_PWM_NODE)

#define GREEN_PWM_NODE DT_NODELABEL(pwm_green)
#define GREEN_PWM_PERIOD DT_PWMS_PERIOD(GREEN_PWM_NODE)

#define BLUE_PWM_NODE DT_NODELABEL(pwm_blue)
#define BLUE_PWM_PERIOD DT_PWMS_PERIOD(BLUE_PWM_NODE)

int main(void)
{
	const struct pwm_dt_spec red_led = PWM_DT_SPEC_GET(RED_PWM_NODE);
	if (!pwm_is_ready_dt(&red_led))
	{
		printf("Failed to find %s device\n", DT_NODE_FULL_NAME(RED_PWM_NODE));
		exit(EXIT_FAILURE);
	}

	const struct pwm_dt_spec green_led = PWM_DT_SPEC_GET(GREEN_PWM_NODE);
	if (!pwm_is_ready_dt(&green_led))
	{
		printf("Failed to find %s device\n", DT_NODE_FULL_NAME(GREEN_PWM_NODE));
		exit(EXIT_FAILURE);
	}

	const struct pwm_dt_spec blue_led = PWM_DT_SPEC_GET(BLUE_PWM_NODE);
	if (!pwm_is_ready_dt(&blue_led))
	{
		printf("Failed to find %s device\n", DT_NODE_FULL_NAME(BLUE_PWM_NODE));
		exit(EXIT_FAILURE);
	}

	console_init();

	printk("Press any key to continue ...");
	(void)console_getchar();
	srand((uint32_t)k_uptime_ticks());

	uint32_t pulse_width;

	while (1)
	{
		pulse_width = rand() % (RED_PWM_PERIOD + 1);
		if (0 != pwm_set_pulse_dt(&red_led, pulse_width))
		{
			printf("Failed to set the red pulse width.\n");
			exit(EXIT_FAILURE);
		}
		printf("R = %08u, ", pulse_width);

		k_sleep(K_NSEC(RED_PWM_PERIOD)); // One period delay to let the driver/hardware clear the 'busy' state
		pulse_width = rand() % (GREEN_PWM_PERIOD + 1);
		if (0 != pwm_set_pulse_dt(&green_led, pulse_width))
		{
			printf("Failed to set the green pulse width.\n");
			exit(EXIT_FAILURE);
		}
		printf("G = %08u, ", pulse_width);

		pulse_width = rand() % (BLUE_PWM_PERIOD + 1);
		if (0 != pwm_set_pulse_dt(&blue_led, pulse_width))
		{
			printf("Failed to set the blue pulse width.\n");
			exit(EXIT_FAILURE);
		}

		printf("B = %08u\n", pulse_width);

		k_sleep(K_MSEC(250));
	}

	return 0;
}