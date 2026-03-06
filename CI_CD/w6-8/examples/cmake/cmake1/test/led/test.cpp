#include "led.h"
#include "config.h"
#include <gtest/gtest.h>

TEST(ConfigRange, MinMax)
{
    EXPECT_EQ(CONFIG_LIGHT_MIN, led::get_light_min());
    EXPECT_EQ(CONFIG_LIGHT_MAX, led::get_light_max());
}

int main(void)
{
    testing::InitGoogleTest();

    return RUN_ALL_TESTS();
}
