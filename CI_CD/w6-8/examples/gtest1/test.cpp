#include <vector>
#include "stack.h"
#include <algorithm>
#include <gtest/gtest.h>

class Memory : public IMemory
{
    std::vector<void *> ptrs;

public:
    Memory() = default;

    void *malloc(size_t size) override
    {
        void *mptr{std::malloc(size)};

        if (mptr != nullptr)
        {
            ptrs.push_back(mptr);
        }

        return mptr;
    }

    void free(void *ptr) override
    {
        ptrs.erase(std::remove(ptrs.begin(), ptrs.end(), ptr), ptrs.end());

        std::free(ptr);
    }

    ~Memory() { EXPECT_EQ(0, ptrs.size()); };
};

TEST(stackTest, invalidMemory)
{
    int value;

    Stack stck{nullptr};
    EXPECT_FALSE(stck.push(10));
    EXPECT_FALSE(stck.pop(value));
    EXPECT_EQ(0, stck.available());
}

TEST(stackTest, testPush)
{
    Memory memory;
    Stack stck{&memory};

    EXPECT_TRUE(stck.push(10));
    EXPECT_EQ(1, stck.available());

    EXPECT_TRUE(stck.push(20));
    EXPECT_EQ(2, stck.available());
}

TEST(stackTest, testPop)
{
    Memory memory;
    Stack stck{&memory};

    EXPECT_TRUE(stck.push(10));
    EXPECT_EQ(1, stck.available());

    EXPECT_TRUE(stck.push(20));
    EXPECT_EQ(2, stck.available());

    int value;

    EXPECT_TRUE(stck.pop(value));
    EXPECT_EQ(1, stck.available());
    EXPECT_EQ(20, value);

    EXPECT_TRUE(stck.pop(value));
    EXPECT_EQ(0, stck.available());
    EXPECT_EQ(10, value);
}

TEST(stackTest, testClear)
{
    Memory memory;
    Stack stck{&memory};

    EXPECT_TRUE(stck.push(10));
    EXPECT_EQ(1, stck.available());

    EXPECT_TRUE(stck.push(20));
    EXPECT_EQ(2, stck.available());

    stck.clear();

    int value;
    EXPECT_FALSE(stck.pop(value));
    EXPECT_EQ(0, stck.available());
}
