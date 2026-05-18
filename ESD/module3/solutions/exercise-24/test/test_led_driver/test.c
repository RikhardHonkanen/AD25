#include "bsp.h"
#include <unity.h>
#include "led_driver.h"
#ifndef DEVENV
#include <unistd.h>
#endif

#define LED_PIN 4

void setUp(void) {}
void tearDown(void) {}

void test_led_driver_initialization(void)
{
    TEST_ASSERT_FALSE(led_driver_set_state(1));

    bsp_set_mode_status(false);
    bsp_set_write_status(false);
    TEST_ASSERT_FALSE(led_driver_init(LED_PIN));

    bsp_set_mode_status(true);
    TEST_ASSERT_FALSE(led_driver_init(LED_PIN));

    bsp_set_write_status(true);
    TEST_ASSERT_TRUE(led_driver_init(LED_PIN));
    TEST_ASSERT_EQUAL_INT(0, bsp_get_pin_state());
    TEST_ASSERT_EQUAL_INT(LED_PIN, bsp_get_pin_num());
    TEST_ASSERT_EQUAL_INT(BSP_GPIO_MODE_OUTPUT, bsp_get_pin_mode());
}

void test_led_driver_set_state(void)
{
    TEST_ASSERT_FALSE(led_driver_set_state(10));

    bsp_set_write_status(false);
    TEST_ASSERT_FALSE(led_driver_set_state(0));
    bsp_set_write_status(true);

#ifndef DEVENV
    sleep(1);
#endif

    TEST_ASSERT_TRUE(led_driver_set_state(1));
    TEST_ASSERT_EQUAL_INT(1, bsp_get_pin_state());
    TEST_ASSERT_EQUAL_INT(LED_PIN, bsp_get_pin_num());

#ifndef DEVENV
    sleep(1);
#endif

    TEST_ASSERT_TRUE(led_driver_set_state(0));
    TEST_ASSERT_EQUAL_INT(0, bsp_get_pin_state());
    TEST_ASSERT_EQUAL_INT(LED_PIN, bsp_get_pin_num());
}

#ifdef DEVENV
int main(void)
#else
void app_main(void)
#endif
{
    UNITY_BEGIN();

    RUN_TEST(test_led_driver_initialization);
    RUN_TEST(test_led_driver_set_state);

#ifdef DEVENV
    return UNITY_END();
#else
    UNITY_END();
#endif
}