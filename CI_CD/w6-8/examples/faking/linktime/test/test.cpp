#include "date.h"
#include "today.h"
#include <gtest/gtest.h>

TEST(TodayTest, testDates)
{
    Today today;

    Date::set("2024-02-29");
    EXPECT_TRUE(today.isLeapDay());

    Date::set("2023-02-29");
    EXPECT_FALSE(today.isLeapDay());
}