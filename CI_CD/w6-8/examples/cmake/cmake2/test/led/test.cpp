#include "led.h"
#include <unity.h>
#include "config.h"

void setUp(void) {}
void tearDown(void) {}

void test_led_config_range(void)
{
    TEST_ASSERT_EQUAL_INT(CONFIG_LIGHT_MIN, led::get_light_min());
    TEST_ASSERT_EQUAL_INT(CONFIG_LIGHT_MAX, led::get_light_max());
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_led_config_range);

    return UNITY_END();
}
