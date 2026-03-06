#include <new>
#include "person.h"
#include <cassert>

struct person_t
{
    int age;
    std::string name;
};

person_t *person::create(const std::string &name, int age)
{
    return new person_t{age, name};
}

void person::set_name(person_t *prsn, const std::string &name)
{
    assert(prsn != nullptr);
    prsn->name = name;
}

void person::set_age(person_t *prsn, int age)
{
    assert(prsn != nullptr);
    prsn->age = age;
}

std::string person::get_name(person_t *prsn)
{
    assert(prsn != nullptr);
    return prsn->name;
}

int person::get_age(person_t *prsn)
{
    assert(prsn != nullptr);
    return prsn->age;
}

void person::destroy(person_t *&prsn)
{
    if (prsn != nullptr)
    {
        delete prsn;
        prsn = nullptr;
    }
}