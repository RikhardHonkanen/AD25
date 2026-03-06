#include "list.h"
#include <array>
#include <algorithm>
#include <gtest/gtest.h>

static constexpr size_t SIZE{10};

class ListFixture : public ::testing::Test
{
protected:
    List list;
    std::array<int, SIZE> array;

    void SetUp(void) override
    {
        std::srand(time(nullptr));

        size_t i = 0;
        while (i < SIZE)
        {
            bool unique{true};
            array[i] = rand() % (SIZE * SIZE);

            for (size_t j = 0; j < i; j++)
            {
                if (array[i] == array[j])
                {
                    unique = false;
                    break;
                }
            }

            if (unique)
            {
                i++;
            }
        }

        EXPECT_EQ(0, list.size());

        for (i = 0; i < SIZE; i++)
        {
            EXPECT_TRUE(list.insert(array[i]));
            EXPECT_EQ(i + 1, list.size());
        }

        EXPECT_EQ(SIZE, list.size());

        std::sort(array.begin(), array.end(), [](auto a, auto b)
                  { return (a < b); });
    }

    void TearDown(void) override {}
};

TEST_F(ListFixture, testClear)
{
    list.clear();
    EXPECT_EQ(0, list.size());
}

TEST_F(ListFixture, testDuplicate)
{
    EXPECT_FALSE(list.insert(array[1]));
    EXPECT_EQ(SIZE, list.size());
}

TEST_F(ListFixture, testSorted)
{
    EXPECT_EQ(0, list.search(array[0] - 1));

    for (size_t i = 0; i < SIZE; i++)
    {
        EXPECT_EQ(i + 1, list.search(array[i]));
    }
}

TEST_F(ListFixture, testRemove)
{
    EXPECT_FALSE(list.remove(array[0] - 1));

    EXPECT_TRUE(list.remove(array[0]));
    EXPECT_EQ(SIZE - 1, list.size());

    EXPECT_TRUE(list.remove(array[1]));
    EXPECT_EQ(SIZE - 2, list.size());
}

TEST_F(ListFixture, testEdit)
{
    EXPECT_FALSE(list.edit(array[0] - 1, array[0]));

    EXPECT_FALSE(list.edit(array[0], array[1]));

    EXPECT_TRUE(list.edit(array[1], array[SIZE - 1] + 1));
    EXPECT_EQ(SIZE, list.size());

    EXPECT_EQ(0, list.search(array[1]));

    array[1] = array[SIZE - 1] + 1;
    std::sort(array.begin(), array.end(), [](auto a, auto b)
              { return (a < b); });

    for (size_t i = 0; i < SIZE; i++)
    {
        EXPECT_EQ(i + 1, list.search(array[i]));
    }
}

TEST_F(ListFixture, testOnlyMovable)
{
    EXPECT_FALSE(std::is_copy_constructible<List>::value);
    EXPECT_FALSE(std::is_copy_assignable<List>::value);

    EXPECT_TRUE(std::is_move_constructible<List>::value);
    EXPECT_TRUE(std::is_move_assignable<List>::value);

    List temp{std::move(list)};

    EXPECT_EQ(0, list.size());
    EXPECT_EQ(SIZE, temp.size());

    list = std::move(temp);

    EXPECT_EQ(0, temp.size());
    EXPECT_EQ(SIZE, list.size());
}