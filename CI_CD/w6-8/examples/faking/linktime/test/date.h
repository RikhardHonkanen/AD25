#ifndef DATE_H
#define DATE_H

#include <string>

class Date
{
    static int year, month, day;

public:
    static void set(const std::string &date);

    int getYear(void) { return year; }
    int getMonth(void) { return month; }
    int getDay(void) { return day; }
};

#endif