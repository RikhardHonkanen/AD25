#include "list.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;

class Memory : public IMemory
{
    size_t pos{0};
    uint8_t array[8192]; // Increase size of array if required
    std::vector<void *> vec;

public:
    MOCK_METHOD(void *, malloc, (size_t size), (override));
    MOCK_METHOD(void, free, (void *ptr), (override));

    void *allocate(size_t size)
    {
        void *ptr{nullptr};

        if ((pos + size) < sizeof(array))
        {
            ptr = &array[pos];
            vec.push_back(ptr);
            pos += size;
        }

        return ptr;
    }

    void release(void *ptr)
    {
        vec.erase(std::remove(vec.begin(), vec.end(), ptr), vec.end());
    }

    ~Memory() { EXPECT_EQ(0, vec.size()); }
};

class ListFixture : public ::testing::Test
{
protected:
    NiceMock<Memory> memory;
    List list{&memory};

    void SetUp(void) override
    {
        EXPECT_EQ(0, list.size());

        ON_CALL(memory, malloc(_))
            .WillByDefault(Invoke(&memory, &Memory::allocate));

        ON_CALL(memory, free(_))
            .WillByDefault(Invoke(&memory, &Memory::release));

        EXPECT_TRUE(list.insert(10));
        EXPECT_EQ(1, list.size());

        EXPECT_TRUE(list.insert(15));
        EXPECT_EQ(2, list.size());

        EXPECT_TRUE(list.insert(20));
        EXPECT_EQ(3, list.size());

        EXPECT_TRUE(list.insert(5));
        EXPECT_EQ(4, list.size());
    }

    void TearDown(void) override {}
};

TEST_F(ListFixture, invalidMemory)
{
    List temp{nullptr};
    EXPECT_EQ(0, temp.size());
    EXPECT_FALSE(temp.insert(50));
    EXPECT_FALSE(temp.remove(50));
}

TEST_F(ListFixture, mallocFails)
{
    EXPECT_CALL(memory, malloc(_))
        .WillOnce(Return(nullptr));
    EXPECT_FALSE(list.insert(50));
}

TEST_F(ListFixture, testClear)
{
    EXPECT_EQ(4, list.size());
    list.clear();
    EXPECT_EQ(0, list.size());
}

TEST_F(ListFixture, testDuplicate)
{
    EXPECT_FALSE(list.insert(15));
    EXPECT_EQ(4, list.size());
}

TEST_F(ListFixture, testSorted)
{
    EXPECT_EQ(0, list.search(1));

    EXPECT_EQ(1, list.search(5));
    EXPECT_EQ(2, list.search(10));
    EXPECT_EQ(3, list.search(15));
    EXPECT_EQ(4, list.search(20));
}

TEST_F(ListFixture, testRemove)
{
    EXPECT_FALSE(list.remove(0));

    EXPECT_TRUE(list.remove(5));
    EXPECT_EQ(3, list.size());

    EXPECT_TRUE(list.remove(15));
    EXPECT_EQ(2, list.size());
}

TEST_F(ListFixture, testEdit)
{
    EXPECT_FALSE(list.edit(5, 15));

    EXPECT_FALSE(list.edit(1, 15));

    EXPECT_TRUE(list.edit(10, 100));
    EXPECT_EQ(4, list.size());

    EXPECT_EQ(0, list.search(10));

    EXPECT_EQ(1, list.search(5));
    EXPECT_EQ(2, list.search(15));
    EXPECT_EQ(3, list.search(20));
    EXPECT_EQ(4, list.search(100));
}

TEST_F(ListFixture, testOnlyMovable)
{
    EXPECT_FALSE(std::is_copy_constructible<List>::value);
    EXPECT_FALSE(std::is_copy_assignable<List>::value);

    EXPECT_TRUE(std::is_move_constructible<List>::value);
    EXPECT_TRUE(std::is_move_assignable<List>::value);

    List temp{std::move(list)};

    EXPECT_EQ(0, list.size());
    EXPECT_EQ(4, temp.size());
    EXPECT_FALSE(list.insert(5));

    list = std::move(temp);

    EXPECT_EQ(0, temp.size());
    EXPECT_EQ(4, list.size());
    EXPECT_FALSE(temp.insert(5));
}