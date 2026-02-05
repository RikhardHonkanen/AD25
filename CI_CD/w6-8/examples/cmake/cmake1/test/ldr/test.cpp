#include "ldr.h"
#include "config.h"
#include <gtest/gtest.h>

TEST(ConfigRange, MinMax)
{
    EXPECT_EQ(CONFIG_RESISTANCE_MIN, ldr::get_resistance_min());
    EXPECT_EQ(CONFIG_RESISTANCE_MAX, ldr::get_resistance_max());
}