#include <stdio.h>

/**
 * @brief
 *
 * Reads a date and prints it to console
 *
 * @return int
 */
struct date
{
    int day;
    int month;
    int year;
    struct date inner_date;
};

// struct person
// {
//     int id;
//     int age;
//     char name[32];
//     struct date birth_date;
// };

int main(void)
{
    struct date date;

    printf("Enter day (DD): \n");
    scanf(" %d", &date.day);

    printf("Enter month (MM): \n");
    scanf(" %d", &date.month);

    printf("Enter year (YYYY): \n");
    scanf(" %d", &date.year);

    printf("You entered: %02d-%02d-%04d\n", date.day, date.month, date.year);
    return 0;
}
