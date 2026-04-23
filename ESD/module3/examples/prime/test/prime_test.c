#include <unity.h>
#include "prime.h"

/**
 * @brief This function is called before running every test function to initialize the environment for the test.
 *        For example if the module needs initialization or to start, we can do it in this function,
 *        or reseting the required variables for the test.
 */
void setUp(void) {}

/**
 * @brief This function is called after running every test function to cleanup the environment.
 *        For example if the module needs to free memory, to be stoped, or etc. we can do it in this function
 */
void tearDown(void) {}

// This is a test function. In these test cases we are going to check for prime numbers
void test_prime_numbers(void)
{
    TEST_ASSERT_TRUE(prime_check(2));
    TEST_ASSERT_TRUE(prime_check(3));
    TEST_ASSERT_TRUE(prime_check(23));
}

// This is another test case. In these test cases we are going to check for non-prime numbers
void test_non_prime_numbers(void)
{
    TEST_ASSERT_FALSE(prime_check(0));
    TEST_ASSERT_FALSE(prime_check(1));
    TEST_ASSERT_FALSE(prime_check(20));
    TEST_ASSERT_FALSE(prime_check(1000));
}

int main(void)
{
    UNITY_BEGIN(); // Start Unity

    // Run the test cases
    RUN_TEST(test_prime_numbers);
    RUN_TEST(test_non_prime_numbers);

    return UNITY_END(); // End Unity
}
