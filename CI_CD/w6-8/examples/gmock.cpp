/**
 * @file gmock.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief To build and run the test: clear && g++ -std=c++14 gmock.cpp -lgtest -lgmock -o main && ./main && rm main
 * @version 0.1
 * @date 2023-01-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

struct IDate
{
    virtual int getYear(void) = 0;
    virtual int getMonth(void) = 0;
    virtual int getDay(void) = 0;
    virtual ~IDate() = default;
};

class MockDate : public IDate
{
public:
    MOCK_METHOD(int, getYear, (), (override));
    MOCK_METHOD(int, getMonth, (), (override));
    MOCK_METHOD(int, getDay, (), (override));
};

class Today
{
    IDate *date{nullptr};

public:
    Today(IDate *idate) : date{idate} {}

    bool isLeapDay(void)
    {
        bool status = false;
        int day = date->getDay();
        int year = date->getYear();
        int month = date->getMonth();
        int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        if ((day > days[month - 1]) && (month == 2))
        {
            if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)))
            {
                if (day == days[month - 1] + 1)
                {
                    status = true;
                }
            }
        }

        return status;
    }
};

TEST(TodayTest, leapDay)
{
    MockDate date;
    Today today{&date};

    EXPECT_CALL(date, getYear()).WillOnce(::testing::Return(2023));
    EXPECT_CALL(date, getMonth()).WillOnce(::testing::Return(2));
    EXPECT_CALL(date, getDay()).WillOnce(::testing::Return(29));
    EXPECT_FALSE(today.isLeapDay());

    EXPECT_CALL(date, getYear()).WillOnce(::testing::Return(2023));
    EXPECT_CALL(date, getMonth()).WillOnce(::testing::Return(3));
    EXPECT_CALL(date, getDay()).WillOnce(::testing::Return(8));
    EXPECT_FALSE(today.isLeapDay());

    EXPECT_CALL(date, getYear()).WillOnce(::testing::Return(2024));
    EXPECT_CALL(date, getMonth()).WillOnce(::testing::Return(2));
    EXPECT_CALL(date, getDay()).WillOnce(::testing::Return(29));
    EXPECT_TRUE(today.isLeapDay());
}

int main(void)
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
