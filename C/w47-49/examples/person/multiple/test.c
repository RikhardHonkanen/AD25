#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "person.h"

int main(void)
{
    char name[PERSON_NAME_MAX_LEN + 1] = {0};

    person_t *stefan = person_create("Stefan", 30);
    assert(stefan != NULL);

    assert(30 == person_get_age(stefan));
    person_get_name(stefan, name);
    assert(0 == strcmp("Stefan", name));
    (void)printf("\nTesting create stefan... Passed\n");

    person_t *eva = person_create("Eva", 23);
    assert(eva != NULL);

    assert(23 == person_get_age(eva));
    person_get_name(eva, name);
    assert(0 == strcmp("Eva", name));
    (void)printf("Testing create eva... Passed\n");

    person_set_name(eva, "Eva Larsson");
    person_get_name(eva, name);
    assert(0 == strcmp("Eva Larsson", name));
    (void)printf("Testing set name... Passed\n");

    person_set_age(eva, 30);
    assert(30 == person_get_age(eva));
    (void)printf("Testing set age... Passed\n");

    person_destroy(&eva);
    assert(NULL == eva);
    person_destroy(&stefan);
    assert(NULL == stefan);
    (void)printf("Testing destroy... Passed\n");

    (void)printf("---------------------------\n");
    (void)printf("All tests Passed!\n\n");

    return 0;
}