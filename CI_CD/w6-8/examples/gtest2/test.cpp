#include "stack.h"
#include <vector>
#include <gtest/gtest.h>

class Memory : public IMemory
{
    bool status{true};
    std::vector<void *> ptrs;

public:
    void setStatus(bool _status)
    {
        status = _status;
    }

    void *malloc(size_t size) override
    {
        void *ptr{nullptr};

        if (status)
        {
            ptr = std::malloc(size);
            if (ptr != nullptr)
            {
                ptrs.push_back(ptr);
            }
        }

        return ptr;
    }

    void free(void *ptr) override
    {
        ptrs.erase(std::remove(ptrs.begin(), ptrs.end(), ptr), ptrs.end());
        std::free(ptr);
    }

    ~Memory() { EXPECT_EQ(0, ptrs.size()); }
};

template <typename T>
class StackTest : public testing::Test
{
protected:
    Memory memory;
    Stack<T> stack{memory};

    void SetUp(void) override {}
    void TearDown(void) override {}
};

using MyTypes = ::testing::Types<int, float>;
TYPED_TEST_SUITE(StackTest, MyTypes);

TYPED_TEST(StackTest, mallocFails)
{
    this->memory.setStatus(false);
    EXPECT_FALSE(this->stack.push(10));
}

TYPED_TEST(StackTest, testPush)
{
    EXPECT_TRUE(this->stack.push(10));
    EXPECT_EQ(1, this->stack.available());

    EXPECT_TRUE(this->stack.push(20));
    EXPECT_EQ(2, this->stack.available());

    EXPECT_TRUE(this->stack.push(30));
    EXPECT_EQ(3, this->stack.available());
}

TYPED_TEST(StackTest, testPop)
{
    TypeParam value;

    EXPECT_TRUE(this->stack.push(10));
    EXPECT_EQ(1, this->stack.available());

    EXPECT_TRUE(this->stack.push(20));
    EXPECT_EQ(2, this->stack.available());

    EXPECT_TRUE(this->stack.push(30));
    EXPECT_EQ(3, this->stack.available());

    EXPECT_TRUE(this->stack.pop(value));
    EXPECT_EQ(2, this->stack.available());
    EXPECT_EQ(30, value);

    EXPECT_TRUE(this->stack.pop(value));
    EXPECT_EQ(1, this->stack.available());
    EXPECT_EQ(20, value);
}

TYPED_TEST(StackTest, testClear)
{
    TypeParam value;

    EXPECT_TRUE(this->stack.push(10));
    EXPECT_EQ(1, this->stack.available());

    EXPECT_TRUE(this->stack.push(20));
    EXPECT_EQ(2, this->stack.available());

    EXPECT_TRUE(this->stack.push(30));
    EXPECT_EQ(3, this->stack.available());

    EXPECT_TRUE(this->stack.pop(value));
    EXPECT_EQ(2, this->stack.available());
    EXPECT_EQ(30, value);

    this->stack.clear();
    EXPECT_FALSE(this->stack.pop(value));
    EXPECT_EQ(0, this->stack.available());
}

TYPED_TEST(StackTest, testMovable)
{
    EXPECT_TRUE(this->stack.push(10));
    EXPECT_TRUE(this->stack.push(20));
    EXPECT_EQ(2, this->stack.available());

    Stack<TypeParam> temp{std::move(this->stack)};

    EXPECT_EQ(0, this->stack.available());

    EXPECT_EQ(2, temp.available());

    TypeParam value;
    EXPECT_TRUE(temp.pop(value));
    EXPECT_EQ(1, temp.available());
    EXPECT_EQ(20, value);

    this->stack = std::move(temp);

    EXPECT_EQ(1, this->stack.available());

    EXPECT_EQ(0, temp.available());

    EXPECT_TRUE(this->stack.pop(value));
    EXPECT_EQ(0, this->stack.available());
    EXPECT_EQ(10, value);
}