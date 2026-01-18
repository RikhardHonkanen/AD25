#include "person.h"

static int age = 0;
static std::string name = "";

void person::init(const std::string &pname, int page)
{
    age = page;
    name = pname;
}

void person::set_name(const std::string &pname)
{
    name = pname;
}

void person::set_age(int page)
{
    age = page;
}

std::string person::get_name(void)
{
    return name;
}

int person::get_age(void)
{
    return age;
}