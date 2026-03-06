#include "ldr.h"
#include <unity.h>
#include "config.h"

void setUp(void) {}
void tearDown(void) {}

void test_ldr_config_range(void)
{
    TEST_ASSERT_EQUAL_INT(CONFIG_RESISTANCE_MIN, ldr::get_resistance_min());
    TEST_ASSERT_EQUAL_INT(CONFIG_RESISTANCE_MAX, ldr::get_resistance_max());
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_ldr_config_range);

    return UNITY_END();
}
