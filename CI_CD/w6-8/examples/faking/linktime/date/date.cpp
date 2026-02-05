#include "date.h"

Date::Date()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t time_temp = std::chrono::system_clock::to_time_t(now);
    const std::tm *local_tm = std::localtime(&time_temp);
    year = local_tm->tm_year + 1900;
    month = local_tm->tm_mon + 1;
    day = local_tm->tm_mday;
}