/**
 * @file main.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Connect an LDR series with a 4.7kOhm resistor to J4:2(P4_6)
 * 			If the LDR resistance is lesser than 2 kOhm, turn the blue LED on
 * 			If the LDR resistance is lesser than 5 kOhm, and greater or equal to 2 kOhm,
 * 			turn the green LED on
 * 			Greater than or equal to 5 kOhm, turn the red LED on
 *
 *        Look at: https://docs.zephyrproject.org/latest/build/dts/index.html
 *                 https://docs.zephyrproject.org/latest/build/dts/api/bindings/led/gpio-leds.html
 *                 https://docs.zephyrproject.org/latest/hardware/peripherals/gpio.html
 *                 https://docs.zephyrproject.org/latest/doxygen/html/group__gpio__interface.html
 *                 https://docs.zephyrproject.org/latest/samples/basic/blinky/README.html
 * @version 0.1
 * @date 2026-04-21
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/__assert.h>

// led0 === red_led
// led1 === green_led
// led2 === blue_led
#define RED_LED_NODE DT_ALIAS(led0)   // The devicetree node identifier for the "led0" alias.
#define GREEN_LED_NODE DT_ALIAS(led1) // The devicetree node identifier for the "led1" alias.
#define BLUE_LED_NODE DT_ALIAS(led2)  // The devicetree node identifier for the "led2" alias.
#define VDD 3300
#define RESISTOR 4700
#define RES_LOW 1000
#define RES_HIGH 3000

int main(void)
{
    const struct adc_dt_spec adc_ldr = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

    if (!adc_is_ready_dt(&adc_ldr) || (0 > adc_channel_setup_dt(&adc_ldr)))
    {
        printk("LDR device is not ready");
        return EXIT_FAILURE;
    }

    int16_t value;
    struct adc_sequence sequence = {
        .buffer = &value,
        .buffer_size = sizeof(value),
        .calibrate = true,
    };

    const struct gpio_dt_spec r_led = GPIO_DT_SPEC_GET(RED_LED_NODE, gpios);
    __ASSERT(gpio_is_ready_dt(&r_led), "GPIO port is not ready!");
    __ASSERT(0 == gpio_pin_configure_dt(&r_led, GPIO_OUTPUT_INACTIVE),
             "Failed to configure the gpio pin");

    const struct gpio_dt_spec g_led = GPIO_DT_SPEC_GET(GREEN_LED_NODE, gpios);
    __ASSERT(gpio_is_ready_dt(&g_led), "GPIO port is not ready!");
    __ASSERT(0 == gpio_pin_configure_dt(&g_led, GPIO_OUTPUT_INACTIVE),
             "Failed to configure the gpio pin");

    const struct gpio_dt_spec b_led = GPIO_DT_SPEC_GET(BLUE_LED_NODE, gpios);
    __ASSERT(gpio_is_ready_dt(&b_led), "GPIO port is not ready!");
    __ASSERT(0 == gpio_pin_configure_dt(&b_led, GPIO_OUTPUT_INACTIVE),
             "Failed to configure the gpio pin");

    int32_t millivolts;

    while (1)
    {
        if ((0 != adc_sequence_init_dt(&adc_ldr, &sequence)) ||
            (0 != adc_read_dt(&adc_ldr, &sequence)))
        {
            printk("ADC failed to read\n");
            return EXIT_FAILURE;
        }
        __ASSERT(0 == gpio_pin_set_dt(&r_led, 0), "Failed to toggle the LED");
        __ASSERT(0 == gpio_pin_set_dt(&g_led, 0), "Failed to toggle the LED");
        __ASSERT(0 == gpio_pin_set_dt(&b_led, 0), "Failed to toggle the LED");

        millivolts = value;
        if (0 > adc_raw_to_millivolts_dt(&adc_ldr, &millivolts))
        {
            printk("TMP ADC failed to convert the raw value to millivolts\n");
            return EXIT_FAILURE;
        }

        int resistance = (RESISTOR * millivolts) / (VDD - millivolts);

        if (resistance < RES_LOW)
        {
            __ASSERT(0 == gpio_pin_toggle_dt(&b_led), "Failed to toggle the LED");
        }
        else if (resistance >= RES_HIGH)
        {
            __ASSERT(0 == gpio_pin_toggle_dt(&r_led), "Failed to toggle the LED");
        }
        else
        {
            __ASSERT(0 == gpio_pin_toggle_dt(&g_led), "Failed to toggle the LED");
        }
    }

    return 0;
}
