#include <unity.h>
#include "console.h"
#include "utility.h"

void test_utility_write_int(void)
{
    console_set_status(false);
    TEST_ASSERT_FALSE(utility_write_int(1234));
    console_set_status(true);

    TEST_ASSERT_TRUE(utility_write_int(0));
    TEST_ASSERT_EQUAL_STRING("0", console_get_output());

    TEST_ASSERT_TRUE(utility_write_int(1234));
    TEST_ASSERT_EQUAL_STRING("1234", console_get_output());

    TEST_ASSERT_TRUE(utility_write_int(-1234));
    TEST_ASSERT_EQUAL_STRING("-1234", console_get_output());
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_utility_write_int);

    return UNITY_END();
}
