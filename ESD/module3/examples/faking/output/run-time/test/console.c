#include "console.h"
#include <string.h>

static size_t indx = 0;
static bool status = true;
static char buffer[CONSOLE_BUFSIZE] = {0};

int console_write(int c)
{
    if (status)
    {
        buffer[indx] = c;
        indx = (indx + 1) % CONSOLE_BUFSIZE;
    }

    return (status ? c : CONSOLE_EOF);
}

void console_set_status(bool _status)
{
    status = _status;
}

const char *console_get_output(void)
{
    static char output[CONSOLE_BUFSIZE];
    memset(output, 0, sizeof(output));
    memcpy(output, buffer, indx);
    memset(buffer, 0, sizeof(buffer));
    indx = 0;

    return output;
}
