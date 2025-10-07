#include <stdio.h>

// struct date
// {
//     int day;
//     int month;
//     int year;
//     struct date inner_date;
// };

// struct person
// {
//     int id;
//     int age;
//     char name[32];
//     struct date birth_date;
// };

// Helper function to create a date that points to itself
// date_t make_self_pointing_date(int day, int month, int year)
// {
//     date_t d = {day, month, year, NULL}; // first init pointer to NULL
//     d.inner_date = &d;                   // now it points to itself
//     return d;
// }

typedef struct date
{
    int day;
    int month;
    int year;
    struct date *inner_date;
} date_t;

/**
 * @brief
 *
 * Reads a date and prints it to console
 *
 * @return int
 */
int main(void)
{
    // struct date date;
    date_t date;
    date.inner_date = &date;

    printf("Enter day (DD): \n");
    scanf(" %d", &date.day);

    printf("Enter month (MM): \n");
    scanf(" %d", &date.month);

    printf("Enter year (YYYY): \n");
    scanf(" %d", &date.year);

    printf("You entered: %02d-%02d-%04d\n", date.day, date.month, date.year);
    printf("Inner date is: %02d-%02d-%04d\n", date.inner_date->day, date.inner_date->month, date.inner_date->year);

    // date_t date_p = make_self_pointing_date(date.day, date.month, date.year);

    // printf("You entered: %02d-%02d-%04d\n", date_p.day, date_p.month, date_p.year);
    // printf("Inner date is: %02d-%02d-%04d\n", date_p.inner_date->day, date_p.inner_date->month, date_p.inner_date->year);
    return 0;
}
