#ifndef IDATE_H
#define IDATE_H

struct IDate
{
    virtual int getYear(void) = 0;
    virtual int getMonth(void) = 0;
    virtual int getDay(void) = 0;
    virtual ~IDate() = default;
};

#endif