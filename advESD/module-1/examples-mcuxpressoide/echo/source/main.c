#include "board.h"
#include <stdio.h>
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

int main(void)
{
    // Init board hardware
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    // Init debug console
    BOARD_InitDebugConsole();

    printf("Hello World!\r\n");

    char chr;

    while (1)
    {
        chr = getchar();
        if (chr == '\r')
        {
            putchar(chr);
            chr = '\n';
        }
        putchar(chr);
    }

    return 0;
}
