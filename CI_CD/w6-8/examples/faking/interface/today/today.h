#ifndef TODAY_H
#define TODAY_H

#include "idate.h"
#include <string>

class Today
{
    IDate &date;

public:
    Today(IDate &idate) : date{idate} {}

    bool isLeapDay(void)
    {
        bool status = false;
        int day = date.getDay();
        int year = date.getYear();
        int month = date.getMonth();
        int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        if ((day > days[month - 1]) && (month == 2))
        {
            if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)))
            {
                if (day == days[month - 1] + 1)
                {
                    status = true;
                }
            }
        }

        return status;
    }

    std::string getDate(void)
    {
        std::string str;

        str = std::to_string(date.getYear()) + "-";
        str += std::to_string(date.getMonth()) + "-";
        str += std::to_string(date.getDay());

        return str;
    }
};

#endif