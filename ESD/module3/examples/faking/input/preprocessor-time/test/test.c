#include <unity.h>
#include "console.h"
#include "utility.h"

void setUp(void) {}
void tearDown(void) {}

void test_utility_read_int_valid_input(void)
{
    int number = 0;

    console_set_input("123\n");
    TEST_ASSERT_TRUE(utility_read_int(&number));
    TEST_ASSERT_EQUAL_INT(123, number);

    console_set_input("+123\n");
    TEST_ASSERT_TRUE(utility_read_int(&number));
    TEST_ASSERT_EQUAL_INT(123, number);

    console_set_input("-123\n");
    TEST_ASSERT_TRUE(utility_read_int(&number));
    TEST_ASSERT_EQUAL_INT(-123, number);

    console_set_input("  123\n");
    TEST_ASSERT_TRUE(utility_read_int(&number));
    TEST_ASSERT_EQUAL_INT(123, number);

    console_set_input("  123 \n");
    TEST_ASSERT_TRUE(utility_read_int(&number));
    TEST_ASSERT_EQUAL_INT(123, number);
}

void test_utility_read_int_invalid_input(void)
{
    int number = 0;

    TEST_ASSERT_FALSE(utility_read_int(NULL));

    console_set_input("\n");
    TEST_ASSERT_FALSE(utility_read_int(&number));

    console_set_input("- ");
    TEST_ASSERT_FALSE(utility_read_int(&number));

    console_set_input("+123A");
    TEST_ASSERT_FALSE(utility_read_int(&number));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_utility_read_int_valid_input);
    RUN_TEST(test_utility_read_int_invalid_input);

    return UNITY_END();
}