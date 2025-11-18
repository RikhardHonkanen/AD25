#include "person.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#if !defined(PERSON_NAME_MAX_LEN) || (PERSON_NAME_MAX_LEN < 8) || (PERSON_NAME_MAX_LEN > 64)
#error PERSON_NAME_MAX_LEN shall be defined as an integer in the range of 8 and 64
#endif

static uint8_t age = 0;
static char name[PERSON_NAME_MAX_LEN + 1] = {0};

void person_init(const char *_name, uint8_t _age)
{
    age = _age;

    assert(_name != NULL);

    int length = strlen(_name);
    (void)strncpy(name, _name, (length > PERSON_NAME_MAX_LEN) ? PERSON_NAME_MAX_LEN : length);
}

void person_set_name(const char *_name)
{
    assert(_name != NULL);

    int length = strlen(_name);
    (void)strncpy(name, _name, (length > PERSON_NAME_MAX_LEN) ? PERSON_NAME_MAX_LEN : length);
}

void person_set_age(uint8_t _age)
{
    age = _age;
}

void person_get_name(char *_name)
{
    assert(_name != NULL);

    (void)strcpy(_name, name);
}

uint8_t person_get_age(void)
{
    return age;
}