#include "date.h"

int Date::year{0};
int Date::month{0};
int Date::day{0};

void Date::set(const std::string &date)
{
    (void)std::sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);
}