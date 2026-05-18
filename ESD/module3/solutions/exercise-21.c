/**
 * @file exercise-21.c
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief Using TDD and Unity test and develop a function to convert a string to an integer.
 *        To build the test, link the installed unity on your computer to the test.
 *
 *        gcc exercise-19.c -lunity -o exercise-19 && ./exercise-19 && rm exercise-19
 *
 * @version 0.1
 * @date 2024-04-08
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <unity.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

static bool strtoint(const char *str, int *const ptr)
{
    bool status = false;

    if ((ptr != NULL) && (str != NULL))
    {
        size_t length = strlen(str);

        if (length > 0)
        {
            if ((str[0] == '-') || (str[0] == '+') || isdigit(str[0]))
            {
                *ptr = 0;
                size_t i = 0;
                status = true;

                if (!isdigit(str[i]))
                {
                    i++;
                }

                while (i < length)
                {
                    if (!isdigit(str[i]))
                    {
                        status = false;
                        *ptr = 0;
                        break;
                    }

                    *ptr *= 10;
                    *ptr += (str[i] - '0');
                    i++;
                }

                if (*ptr > 0)
                {
                    if (str[0] == '-')
                    {
                        *ptr *= -1;
                    }
                }
            }
        }
    }

    return status;
}

void setUp() {}
void tearDown() {}

void test_invalid_arguments(void)
{
    int number;
    TEST_ASSERT_FALSE(strtoint("", NULL));
    TEST_ASSERT_FALSE(strtoint(NULL, NULL));
    TEST_ASSERT_FALSE(strtoint(NULL, &number));
}

void test_invalid_string(void)
{
    int number;
    TEST_ASSERT_FALSE(strtoint("", &number));
    TEST_ASSERT_FALSE(strtoint("?23", &number));
    TEST_ASSERT_FALSE(strtoint("a23", &number));
    TEST_ASSERT_FALSE(strtoint("--12   ", &number));
}

void test_valid_string(void)
{
    int number;

    TEST_ASSERT_TRUE(strtoint("0", &number));
    TEST_ASSERT_EQUAL_INT(0, number);

    TEST_ASSERT_TRUE(strtoint("00", &number));
    TEST_ASSERT_EQUAL_INT(0, number);

    TEST_ASSERT_TRUE(strtoint("+00", &number));
    TEST_ASSERT_EQUAL_INT(0, number);

    TEST_ASSERT_TRUE(strtoint("-00", &number));
    TEST_ASSERT_EQUAL_INT(0, number);

    TEST_ASSERT_TRUE(strtoint("-123", &number));
    TEST_ASSERT_EQUAL_INT(-123, number);

    TEST_ASSERT_TRUE(strtoint("+123", &number));
    TEST_ASSERT_EQUAL_INT(123, number);

    TEST_ASSERT_TRUE(strtoint("123", &number));
    TEST_ASSERT_EQUAL_INT(123, number);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_invalid_arguments);
    RUN_TEST(test_invalid_string);
    RUN_TEST(test_valid_string);

    return UNITY_END();
}
