#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <esp_attr.h>
#include <driver/ledc.h>
#include <driver/gptimer.h>

constexpr int FADING_STEP{5};
constexpr int INTERVAL{50000}; // 50ms interval

constexpr ledc_timer_t LEDC_TIMER{LEDC_TIMER_0};
constexpr ledc_channel_t LEDC_CHANNEL{LEDC_CHANNEL_0};
constexpr ledc_timer_bit_t LEDC_RESOLUTION{LEDC_TIMER_13_BIT}; // PWM resolution
constexpr uint32_t LEDC_FREQUENCY{100U};                       // PWM frequency in Hertz

static inline uint32_t percent_to_value(int percent)
{
    return ((percent / 100.0f) * (1 << LEDC_RESOLUTION));
}

static bool timer_on_alarm(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    (void)edata;
    (void)timer;
    (void)user_data;

    static int step, duty = 0;

    if (duty == 100)
    {
        step = -FADING_STEP;
    }
    else if (duty == 0)
    {
        step = FADING_STEP;
    }
    else
    {
        ;
    }

    duty += step;
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL, percent_to_value(duty))); // Set duty cycle
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL));                      // Apply the new duty cycle

    return true; // Whether a high priority task will be awakened up by this function
}

extern "C" void app_main(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer;
    memset(&ledc_timer, 0, sizeof(ledc_timer));
    ledc_timer.timer_num = LEDC_TIMER;
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer.freq_hz = LEDC_FREQUENCY;
    ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_timer.duty_resolution = LEDC_RESOLUTION;

    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel;
    memset(&ledc_channel, 0, sizeof(ledc_channel));
    ledc_channel.gpio_num = GPIO_NUM_4;
    ledc_channel.channel = LEDC_CHANNEL;
    ledc_channel.timer_sel = LEDC_TIMER;
    ledc_channel.intr_type = LEDC_INTR_DISABLE;
    ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // Create timer handle
    gptimer_handle_t gptimer = nullptr;
    gptimer_config_t timer_config;
    memset(&timer_config, 0, sizeof(timer_config));
    timer_config.resolution_hz = 1000000; // 1MHz, 1 tick = 1us
    timer_config.direction = GPTIMER_COUNT_UP;
    timer_config.clk_src = GPTIMER_CLK_SRC_DEFAULT;

    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    // Register the event handler
    gptimer_event_callbacks_t cbs{timer_on_alarm};
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, nullptr));

    // Enable the timer
    ESP_ERROR_CHECK(gptimer_enable(gptimer));

    // Set the alarm and start the timer
    gptimer_alarm_config_t alarm_config{};
    alarm_config.reload_count = 0;
    alarm_config.alarm_count = INTERVAL; // period = 50ms
    alarm_config.flags.auto_reload_on_alarm = true;
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
    ESP_ERROR_CHECK(gptimer_start(gptimer));

    while (1)
    {
        sleep(1);
    }
}
