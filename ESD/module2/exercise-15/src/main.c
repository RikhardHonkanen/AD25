#include <zephyr/kernel.h>
#include <zephyr/drivers/led.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/counter.h>

#define FADING_STEP 1
#define INTERVAL 30000
#define ALARM_CHANNEL_ID 0
#define TIMER DT_NODELABEL(ctimer0)

LOG_MODULE_REGISTER(pwmled);

const struct led_dt_spec pwm_led = LED_DT_SPEC_GET(DT_NODELABEL(myled));

static void on_alarm(const struct device *counter_dev, uint8_t chan_id, uint32_t ticks, void *user_data)
{
	ARG_UNUSED(ticks);
	ARG_UNUSED(chan_id);

	static int8_t step = FADING_STEP;
	static uint8_t duty = FADING_STEP;

	if ((duty == 0) || (duty == 100))
	{
		step *= -1;
	}

	duty += step;

	if (0 != led_set_brightness_dt(&pwm_led, duty))
	{
		LOG_ERR("Failed to set the LED brightness");
	}

	if (0 != counter_set_channel_alarm(counter_dev, ALARM_CHANNEL_ID, user_data))
	{
		LOG_ERR("Failed to set alarm");
	}
}

int main(void)
{
	if (!led_is_ready_dt(&pwm_led))
	{
		LOG_ERR("LED device %s is not ready", pwm_led.dev->name);
		exit(EXIT_FAILURE);
	}

	const struct device *const counter_dev = DEVICE_DT_GET(TIMER);
	if (!device_is_ready(counter_dev))
	{
		LOG_ERR("Error: didn't find %s device", DT_NODE_FULL_NAME(TIMER));
		exit(EXIT_FAILURE);
	}

	counter_start(counter_dev);

	const struct counter_alarm_cfg alarm_cfg = {
		.callback = on_alarm,
		.ticks = counter_us_to_ticks(counter_dev, INTERVAL),
		.user_data = (void *)&alarm_cfg,
	};

	if (0 != counter_set_channel_alarm(counter_dev, ALARM_CHANNEL_ID, &alarm_cfg))
	{
		LOG_ERR("Failed to set alarm");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		k_sleep(K_FOREVER);
	}

	return EXIT_SUCCESS;
}