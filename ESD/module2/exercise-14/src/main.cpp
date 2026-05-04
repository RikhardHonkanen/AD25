#include <zephyr/kernel.h>
#include <zephyr/sys_clock.h>
#include <zephyr/drivers/led.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(myled);

class Metro
{
	uint64_t prev_micros;
	const uint64_t interval;

public:
	Metro(uint64_t _interval) : prev_micros{k_ticks_to_us_floor64(k_uptime_ticks())}, interval{1000 * _interval} {}

	bool check(void)
	{
		bool checked{false};
		uint64_t now{k_ticks_to_us_floor64(k_uptime_ticks())};

		if (now - prev_micros >= interval)
		{
			checked = true;
			prev_micros = now;
		}

		return checked;
	}
};

int main(void)
{
	const struct led_dt_spec led_dev = LED_DT_SPEC_GET(DT_NODELABEL(myled));

	if (!led_is_ready_dt(&led_dev))
	{
		LOG_ERR("LED device %s is not ready", led_dev.dev->name);
		exit(EXIT_FAILURE);
	}

	Metro blink{500};
	Metro hello{1000};

	while (1)
	{
		if (blink.check())
		{
			static bool state{true};
			if (0 != (state ? led_on_dt(&led_dev) : led_off_dt(&led_dev)))
			{
				LOG_ERR("Failed to toggle the LED");
				exit(EXIT_FAILURE);
			}
			state = !state;
		}

		if (hello.check())
		{
			printf("Hello World\r\n");
		}
	}

	return EXIT_SUCCESS;
}