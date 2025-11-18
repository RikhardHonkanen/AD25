#include "person.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#if !defined(PERSON_NAME_MAX_LEN) || (PERSON_NAME_MAX_LEN < 8) || (PERSON_NAME_MAX_LEN > 64)
#error PERSON_NAME_MAX_LEN shall be defined as an integer in the range of 8 and 64
#endif

struct person
{
    uint8_t age;
    char name[PERSON_NAME_MAX_LEN + 1];
};

person_t *person_create(const char *name, uint8_t age)
{
    assert(name != NULL);
    person_t *prsn = (person_t *)malloc(sizeof(person_t));

    if (prsn != NULL)
    {
        prsn->age = age;
        int length = strlen(name);
        (void)strncpy(prsn->name, name, (length > PERSON_NAME_MAX_LEN) ? PERSON_NAME_MAX_LEN : length);
    }

    return prsn;
}

void person_set_name(person_t *prsn, const char *name)
{
    assert((prsn != NULL) && (name != NULL));

    int length = strlen(name);
    (void)strncpy(prsn->name, name, (length > PERSON_NAME_MAX_LEN) ? PERSON_NAME_MAX_LEN : length);
}

void person_set_age(person_t *prsn, uint8_t age)
{
    assert(prsn != NULL);
    prsn->age = age;
}

void person_get_name(person_t *prsn, char *name)
{
    assert((prsn != NULL) && (name != NULL));
    (void)strcpy(name, prsn->name);
}

uint8_t person_get_age(person_t *prsn)
{
    assert(prsn != NULL);
    return prsn->age;
}

void person_destroy(person_t **prsn)
{
    assert((prsn != NULL) && (*prsn != NULL));

    free(*prsn);
    *prsn = NULL;
}