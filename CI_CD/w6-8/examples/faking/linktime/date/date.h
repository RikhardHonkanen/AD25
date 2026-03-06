#ifndef DATE_H
#define DATE_H

#include <ctime>
#include <chrono>

class Date
{
    int day, month, year;

public:
    Date();

    int getYear(void) { return year; }
    int getMonth(void) { return month; }
    int getDay(void) { return day; }
};

#endif