#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "person.h"

int main(void)
{
    char name[PERSON_NAME_MAX_LEN + 1] = {0};

    assert(0 == person_get_age());
    person_get_name(name);
    assert(0 == strcmp("", name));
    (void)printf("\nTesting uninit... Passed\n");

    person_init("Eva", 21);

    assert(21 == person_get_age());
    person_get_name(name);
    assert(0 == strcmp("Eva", name));
    (void)printf("Testing init... Passed\n");

    person_set_name("Stefan");
    person_get_name(name);
    assert(0 == strcmp("Stefan", name));
    (void)printf("Testing set name... Passed\n");

    person_set_age(30);
    assert(30 == person_get_age());
    (void)printf("Testing set age... Passed\n");

    (void)printf("---------------------------\n");
    (void)printf("All tests Passed!\n\n");

    return 0;
}