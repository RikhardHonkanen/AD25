#include <unity.h>
#include <string.h>
#include "console.h"
#include "utility.h"

#define STRLEN 8

void test_utility_string_read(void)
{
    char string[STRLEN + 1];

    TEST_ASSERT_FALSE(utility_string_read(NULL, 0));
    TEST_ASSERT_FALSE(utility_string_read(string, 0));
    TEST_ASSERT_FALSE(utility_string_read(NULL, STRLEN));

    console_set_status(false);
    TEST_ASSERT_FALSE(utility_string_read(string, STRLEN));
    console_set_status(true);

    console_set_input("ABCD\n");
    TEST_ASSERT_TRUE(utility_string_read(string, STRLEN));
    TEST_ASSERT_EQUAL_STRING("ABCD", string);

    console_set_input("abcdefgh\n");
    TEST_ASSERT_TRUE(utility_string_read(string, STRLEN));
    TEST_ASSERT_EQUAL_STRING("abcdefgh", string);
}

void test_utility_string_reverse(void)
{
    char string[STRLEN + 1];

    utility_string_reverse(NULL);

    strcpy(string, "");
    utility_string_reverse(string);
    TEST_ASSERT_EQUAL_STRING("", string);

    strcpy(string, "A");
    utility_string_reverse(string);
    TEST_ASSERT_EQUAL_STRING("A", string);

    strcpy(string, "ABC");
    utility_string_reverse(string);
    TEST_ASSERT_EQUAL_STRING("CBA", string);
}

void test_utility_string_write(void)
{
    TEST_ASSERT_FALSE(utility_string_write(NULL));
    console_set_status(false);
    TEST_ASSERT_FALSE(utility_string_write("AB"));
    console_set_status(true);

    TEST_ASSERT_TRUE(utility_string_write("Hello"));
    TEST_ASSERT_EQUAL_STRING("Hello", console_get_output());
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_utility_string_read);
    RUN_TEST(test_utility_string_reverse);
    RUN_TEST(test_utility_string_write);

    return UNITY_END();
}
