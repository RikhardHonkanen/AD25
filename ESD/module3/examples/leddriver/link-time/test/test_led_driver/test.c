#include <unity.h>
#include "led_driver.h"
#include "driver/gpio.h"

#define LED_PIN 4

void setUp(void) {}
void tearDown(void) {}

void test_led_driver_initialization(void)
{
    TEST_ASSERT_FALSE(led_driver_set_state(1));

    gpio_set_dir_status(false);
    gpio_set_level_status(false);
    gpio_set_reset_status(false);
    TEST_ASSERT_FALSE(led_driver_init(LED_PIN));

    gpio_set_reset_status(true);
    TEST_ASSERT_FALSE(led_driver_init(LED_PIN));

    gpio_set_dir_status(true);
    TEST_ASSERT_FALSE(led_driver_init(LED_PIN));

    gpio_set_level_status(true);
    TEST_ASSERT_TRUE(led_driver_init(LED_PIN));
    TEST_ASSERT_EQUAL_INT(LED_PIN, gpio_get_pin_num());
    TEST_ASSERT_EQUAL_INT(GPIO_MODE_INPUT_OUTPUT, gpio_get_pin_mode());
    TEST_ASSERT_EQUAL_INT(0, gpio_get_level(LED_PIN));
}

void test_led_driver_set_state(void)
{
    TEST_ASSERT_FALSE(led_driver_set_state(10));

    TEST_ASSERT_TRUE(led_driver_set_state(0));
    TEST_ASSERT_EQUAL_INT(LED_PIN, gpio_get_pin_num());
    TEST_ASSERT_EQUAL_INT(0, gpio_get_level(LED_PIN));

    TEST_ASSERT_TRUE(led_driver_set_state(1));
    TEST_ASSERT_EQUAL_INT(LED_PIN, gpio_get_pin_num());
    TEST_ASSERT_EQUAL_INT(1, gpio_get_level(LED_PIN));

    gpio_set_level_status(false);
    TEST_ASSERT_FALSE(led_driver_set_state(0));
    TEST_ASSERT_EQUAL_INT(LED_PIN, gpio_get_pin_num());
    gpio_set_level_status(true);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_led_driver_initialization);
    RUN_TEST(test_led_driver_set_state);

    return UNITY_END();
}