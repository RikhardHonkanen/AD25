/**
 * @file test.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of blackbox testing.
 *        To build and run the test: gcc blockbox.c -lunity -o test && ./test
 * @version 0.1
 * @date 2025-04-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <unity.h>
#include <string.h>

#define MAX_LENGTH 64
static char actual[MAX_LENGTH];

void setUp(void) { memset(actual, 0, sizeof(actual)); }
void tearDown(void) {}

void test_strlen(void)
{
    TEST_ASSERT_EQUAL_UINT64(0, strlen(""));
    TEST_ASSERT_EQUAL_UINT64(11, strlen("Test String"));
}

void test_sprintf(void)
{
    sprintf(actual, "%s", "");
    TEST_ASSERT_EQUAL_STRING("", actual);

    sprintf(actual, "Insert %s", "String");
    TEST_ASSERT_EQUAL_STRING("Insert String", actual);

    sprintf(actual, "%d + %d %c %d", 20, 30, '=', 50);
    TEST_ASSERT_EQUAL_STRING("20 + 30 = 50", actual);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_strlen);
    RUN_TEST(test_sprintf);

    return UNITY_END();
}