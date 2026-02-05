#include "today.h"
#include <gtest/gtest.h>

class Date : public IDate
{
    int year, month, day;

public:
    Date() : year{0}, month{0}, day{0} {}

    void set(const std::string &date)
    {
        (void)std::sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);
    }

    int getYear(void) override { return year; }
    int getMonth(void) override { return month; }
    int getDay(void) override { return day; }
};

TEST(TodayTest, testDates)
{
    Date date;
    Today today{date};

    date.set("2024-02-29");
    EXPECT_TRUE(today.isLeapDay());

    date.set("2023-02-29");
    EXPECT_FALSE(today.isLeapDay());
}