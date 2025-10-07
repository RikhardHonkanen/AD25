#include <stdio.h>

/**
 * @brief
 * Prints "Hello World"
 *
 * @return int
 */
typedef struct
{
    int day;
    int month;
    int year;
} date_t;

int main(void)
{
    date_t date;

    printf("Enter day (DD): \n");
    scanf(" %d", &date.day);

    printf("Enter month (MM): \n");
    scanf(" %d", &date.month);

    printf("Enter year (YYYY): \n");
    scanf(" %d", &date.year);

    printf("You entered: %02d-%02d-%04d\n", date.day, date.month, date.year);
    return 0;
}
