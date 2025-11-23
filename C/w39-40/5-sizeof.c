#include <stdio.h>
#include <limits.h>
#include <float.h>

enum color
{
    RED,
    GREEN,
    BLUE,
    YELLOW,
    WHITE,
    BLACK
};

/**
 * @brief
 * Prints size and min/max values of C data types
 *
 * @return int
 */
int main(void)
{
    // Char types
    printf("Sizeof char: %zu\n", sizeof(char));
    printf("Minimum char: %d\n", CHAR_MIN);
    printf("Maximum char: %d\n", CHAR_MAX);

    printf("Sizeof signed char: %zu\n", sizeof(signed char));
    printf("Minimum signed char: %d\n", SCHAR_MIN);
    printf("Maximum signed char: %d\n", SCHAR_MAX);

    printf("Sizeof unsigned char: %zu\n", sizeof(unsigned char));
    printf("Minimum unsigned char: %d\n", 0);
    printf("Maximum unsigned char: %d\n", UCHAR_MAX);

    // Integer types
    printf("Sizeof int: %zu\n", sizeof(int));
    printf("Minimum int: %d\n", INT_MIN);
    printf("Maximum int: %d\n", INT_MAX);

    printf("Sizeof unsigned int: %zu\n", sizeof(unsigned int));
    printf("Maximum unsigned int: %u\n", UINT_MAX);

    printf("Sizeof short int: %zu\n", sizeof(short int));
    printf("Minimum short int: %d\n", SHRT_MIN);
    printf("Maximum short int: %d\n", SHRT_MAX);

    printf("Sizeof unsigned short int: %zu\n", sizeof(unsigned short int));
    printf("Maximum unsigned short int: %u\n", USHRT_MAX);

    printf("Sizeof long int: %zu\n", sizeof(long int));
    printf("Minimum long int: %ld\n", LONG_MIN);
    printf("Maximum long int: %ld\n", LONG_MAX);

    printf("Sizeof unsigned long int: %zu\n", sizeof(unsigned long int));
    printf("Maximum unsigned long int: %lu\n", ULONG_MAX);

    printf("Sizeof long long int: %zu\n", sizeof(long long int));
    printf("Minimum long long int: %lld\n", LLONG_MIN);
    printf("Maximum long long int: %lld\n", LLONG_MAX);

    printf("Sizeof unsigned long long int: %zu\n", sizeof(unsigned long long int));
    printf("Maximum unsigned long long int: %llu\n", ULLONG_MAX);

    // Floating point types
    printf("Sizeof float: %zu\n", sizeof(float));
    printf("Minimum float: %e\n", FLT_MIN);
    printf("Maximum float: %e\n", FLT_MAX);

    printf("Sizeof double: %zu\n", sizeof(double));
    printf("Minimum double: %e\n", DBL_MIN);
    printf("Maximum double: %e\n", DBL_MAX);

    printf("Sizeof long double: %zu\n", sizeof(long double));
    printf("Minimum long double: %.36Le\n", LDBL_MIN);
    printf("Maximum long double: %.36Le\n", LDBL_MAX);

    // Enum size
    printf("Sizeof enum color: %zu\n", sizeof(enum color));

    return 0;
}