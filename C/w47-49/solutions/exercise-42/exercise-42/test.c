#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "utility.h"

#define STRLEN 19U

void test_get_lcm(void)
{
    assert(0 == utility_get_lcm(0, 0));
    assert(0 == utility_get_lcm(10, 0));
    assert(0 == utility_get_lcm(0, 10));

    assert(12 == utility_get_lcm(4, 6));
    assert(24 == utility_get_lcm(12, 8));
    assert(144 == utility_get_lcm(48, 18));
}

void test_get_gcd(void)
{
    assert(0 == utility_get_gcd(0, 0));
    assert(0 == utility_get_gcd(10, 0));
    assert(0 == utility_get_gcd(0, 10));

    assert(2 == utility_get_gcd(4, 6));
    assert(4 == utility_get_gcd(12, 8));
    assert(6 == utility_get_gcd(48, 18));
}

void test_sort_string(void)
{
    char string[STRLEN + 1] = {0};

    utility_sort_string(string);
    assert(0 == strcmp("", string));

    (void)strncpy(string, "xAYqeMca", STRLEN);
    utility_sort_string(string);
    assert(0 == strcmp("AaceMqxY", string));

    (void)strncpy(string, "XayQEmCA", STRLEN);
    utility_sort_string(string);
    assert(0 == strcmp("AaCEmQXy", string));

    (void)strncpy(string, "jsaNSHosAlPWnSJq", STRLEN);
    utility_sort_string(string);
    assert(0 == strcmp("AaHJjlNnoPqSSssW", string));
}

int main(void)
{
    (void)printf("Testing get_lcm: ");
    test_get_lcm();
    (void)printf("passed!\n");

    (void)printf("Testing get_gcd: ");
    test_get_gcd();
    (void)printf("passed!\n");

    (void)printf("Testing sort_string: ");
    test_sort_string();
    (void)printf("passed!\n");

    (void)printf("---------------------------\n");
    (void)printf("All tests passed!\n");

    return 0;
}
