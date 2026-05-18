#include "stdlib.h"
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define MAX_MALLOC_CALLS 4 // Max. number of calls to malloc by the tests

static size_t counter;                   // A counter used to store non-NULL pointers returned by the fake malloc
static void *malloc_ptr;                 // A pointer used as top of the memory pool to allocate memory by the fake malloc.
static uint8_t buffer[4096];             // A buffer used as memory pool to emulate the heap memory
static void *pointers[MAX_MALLOC_CALLS]; /* An array to store all the poinetrs which point to successfully allocated memories by the fake malloc.
                                            We use this array to check if all the allocated memories are freed by the destroy function or not. */
static bool current, next;               // Variables to control the behaviour of the fake malloc If we want to force the fake malloc to fail

void stdlib_init(void)
{
    counter = 0;
    malloc_ptr = buffer;
    current = next = true;
    memset(pointers, 0, sizeof(pointers)); // Set all the pointers to NULL
}

void stdlib_set_malloc_status(bool first, bool second)
{
    current = first;
    next = second;
}

// The test double to spy on malloc
void *malloc(size_t size)
{
    void *ptr = NULL;

    if (counter < MAX_MALLOC_CALLS) // Ensure that we have a valid value for MAX_MALLOC_CALLS
    {
        if (current)
        {
            current = next;

            ptr = malloc_ptr;
            malloc_ptr = ((uint8_t *)malloc_ptr + size);

            pointers[counter] = ptr; // Store the pointer in the array of pointers
            counter++;
        }
    }

    return ptr;
}

// The test double to spy on free
void free(void *ptr)
{
    if (ptr != NULL)
    {
        // Find the the pointer and set it to NULL
        for (size_t i = 0; i < MAX_MALLOC_CALLS; i++)
        {
            if (pointers[i] == ptr)
            {
                pointers[i] = NULL;
                break;
            }
        }
    }
}

bool stdlib_all_blocks_released(void)
{
    bool status = true;

    for (size_t i = 0; i < MAX_MALLOC_CALLS; i++)
    {
        if (pointers[i] != NULL)
        {
            status = false;
            break;
        }
    }

    return status;
}