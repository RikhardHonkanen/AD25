#include "console.h"
#include <string.h>

typedef struct
{
    size_t pos;
    char buffer[CONSOLE_BUFSIZE];
} buffer_t;

static bool status = true;
static buffer_t input, output;

int console_write(int c)
{
    if (status)
    {
        output.buffer[output.pos] = c;
        output.pos = (output.pos + 1) % CONSOLE_BUFSIZE;
    }

    return (status ? c : CONSOLE_EOF);
}

int console_read(void)
{
    int chr = CONSOLE_EOF;

    if (status)
    {
        chr = input.buffer[input.pos];
        input.pos = (input.pos + 1) % CONSOLE_BUFSIZE;
    }

    return chr;
}

void console_set_status(bool _status)
{
    status = _status;
}

const char *console_get_output(void)
{
    static char buffer[CONSOLE_BUFSIZE];

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, output.buffer);
    memset(&output, 0, sizeof(output));

    return buffer;
}

void console_set_input(const char *str)
{
    memset(&input, 0, sizeof(input));
    strcpy(input.buffer, str);
}
