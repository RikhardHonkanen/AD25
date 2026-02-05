/**
 * @file test.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief To build and run the test: clear && g++ -std=c++14 stack.cpp test.cpp -lgtest -lgmock -o main && ./main && rm main
 * @version 0.1
 * @date 2023-01-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <vector>
#include "stack.h"
#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;

class MockMemory : public IMemory
{
    std::vector<void *> ptrs;

public:
    MOCK_METHOD(void *, malloc, (size_t size), (override));
    MOCK_METHOD(void, free, (void *ptr), (override));

    void *allocate(size_t size)
    {
        void *mptr{std::malloc(size)};

        if (mptr != nullptr)
        {
            ptrs.push_back(mptr);
        }

        return mptr;
    }

    void release(void *ptr)
    {
        ptrs.erase(std::remove(ptrs.begin(), ptrs.end(), ptr), ptrs.end());

        std::free(ptr);
    }

    ~MockMemory() { EXPECT_EQ(0, ptrs.size()); };
};

class StackFixture : public testing::Test
{
protected:
    NiceMock<MockMemory> mock;
    stack::stack_t *stck{nullptr};

    void SetUp(void) override
    {
        stack::init(&mock);

        ON_CALL(mock, malloc(_))
            .WillByDefault(Invoke(&mock, &MockMemory::allocate));

        ON_CALL(mock, free(_))
            .WillByDefault(Invoke(&mock, &MockMemory::release));

        stck = stack::create(8);
        EXPECT_NE(nullptr, stck);
    }

    void TearDown(void) override
    {
        stack::destroy(stck);
    }
};

TEST_F(StackFixture, invalidIMemory)
{
    stack::init(nullptr);
    EXPECT_EQ(nullptr, stack::create(10));
    stack::init(&mock);
}

TEST_F(StackFixture, firstMallocFailed)
{
    EXPECT_CALL(mock, malloc(_))
        .WillOnce(Return(nullptr));
    EXPECT_EQ(nullptr, stack::create(10));
}

TEST_F(StackFixture, secondMallocFailed)
{
    EXPECT_CALL(mock, malloc(_))
        .Times(2)
        .WillOnce(Invoke(&mock, &MockMemory::allocate))
        .WillOnce(Return(nullptr));
    EXPECT_EQ(nullptr, stack::create(10));
}

TEST_F(StackFixture, testPush)
{
    EXPECT_TRUE(stack::push(stck, 10));
    EXPECT_EQ(1, stack::available(stck));

    EXPECT_TRUE(stack::push(stck, 20));
    EXPECT_EQ(2, stack::available(stck));
}

TEST_F(StackFixture, testPop)
{
    EXPECT_TRUE(stack::push(stck, 10));
    EXPECT_EQ(1, stack::available(stck));

    EXPECT_TRUE(stack::push(stck, 20));
    EXPECT_EQ(2, stack::available(stck));

    int value;

    EXPECT_TRUE(stack::pop(stck, value));
    EXPECT_EQ(1, stack::available(stck));
    EXPECT_EQ(20, value);

    EXPECT_TRUE(stack::pop(stck, value));
    EXPECT_EQ(0, stack::available(stck));
    EXPECT_EQ(10, value);
}

TEST_F(StackFixture, testClear)
{
    EXPECT_TRUE(stack::push(stck, 10));
    EXPECT_EQ(1, stack::available(stck));

    EXPECT_TRUE(stack::push(stck, 20));
    EXPECT_EQ(2, stack::available(stck));

    stack::clear(stck);

    int value;
    EXPECT_FALSE(stack::pop(stck, value));
    EXPECT_EQ(0, stack::available(stck));
}

int main(void)
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}