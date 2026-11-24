/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of semihosting. Semihosting is a mechanism that allows the microcontroller to communicate with
          a host computer during development or debugging. It enables the target to use the host's resources, such as file I/O,
          console output, or input, without requiring the target to have full implementations of these features.
          Common Uses: debugging output, file I/O, user input and system calls.
 *
 *        You can do the following settings:
 *           - Quick Settings -> SDK Debug Console: to choose UART or Semihost as the debugging console
 *           - Quick Settings -> Set library/header type: to choose Redlib/Newlib, and nohost/semihost support
 *           - Right click on the project and then click on
 *                 - Properties -> C/C++ Build -> Settings -> Tool Settings -> MCU C Compiler -> Preprocessor
 *                      - To enable/disable redirecting the debug console IO functions to UART, add/remove SDK_DEBUGCONSOLE_UART
 *                      - To select the C library I/O functions like printf and scanf, set SDK_DEBUGCONSOLE value 0
 *                      - To select the debug console functions like PRINTF and SCANF, set SDK_DEBUGCONSOLE value 1
 *                      - To disable debug console functions, set SDK_DEBUGCONSOLE value 2
 *           - To change settings you need to select the project and after changing clear and rebuild the project
 *
 * @version 0.1
 * @date 2025-10-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include "board.h"
#include <stdlib.h>
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

static void semihosting_file(void)
{
	FILE *file = fopen("example.txt", "w");

    if (file != NULL)
    {
        const char msg[] = "Hello from MCXN236!";
        printf("Message: %s\r\n", msg);

        printf("Writing the message to the file...\r\n");
        size_t length = strlen(msg);

        if(length != fwrite(msg, sizeof(msg[0]), length, file))
        {
        	printf("Failed to write the message to the file.\r\n\r\n");
        }

        fclose(file);
    }

	file = fopen("example.txt", "r");

    if (file != NULL)
    {
        char msg[32] = {0};
        printf("Reading the message from the file...\n");

        if(0 < fread(msg, sizeof(msg[0]), sizeof(msg) - 1, file))
        {
        	printf("Message: %s\r\n\r\n", msg);
        }
        else
        {
        	printf("Failed to read from the file.\r\n\r\n");
        }

        fclose(file);
    }
}

static void semihosting_time(void)
{
	time_t timestamp = time(NULL);

	if (timestamp > 0)
	{
		printf("Host Unix timestamp: %lld\r\n", timestamp);

		char string[64];
        strcpy(string, ctime(&timestamp));
        printf("Formatted Time: %s\r\n", string);

        struct tm *timeinfo = localtime(&timestamp);
        if (timeinfo != NULL)
        {
        	printf("Time info:\r\n");
            printf("    Year: %d\r\n", timeinfo->tm_year + 1900);
            printf("   Month: %d\r\n", timeinfo->tm_mon + 1);
            printf("     Day: %d\r\n", timeinfo->tm_mday);
            printf("    Hour: %d\r\n", timeinfo->tm_hour);
            printf("  Minute: %d\r\n", timeinfo->tm_min);
            printf("  Second: %d\r\n", timeinfo->tm_sec);
            strftime(string, sizeof(string), "%Z", timeinfo);
            printf("Timezone: %s\r\n\r\n", string);
        }
	}
}

int main(void)
{
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();

	semihosting_file();

	semihosting_time();

	char chr;

	while (1)
	{
		// The C library IO functions
		printf("Press any key to print it using putchar: ");
		chr = getchar();
		if (chr == '\n')
		{
			chr = getchar();
		}
		if ((chr != '\r') && (chr != EOF))
		{
			putchar(chr);
		}
		printf("\r\n");

		// The Debug Console IO functions
		PRINTF("PRESS ANY KEY TO PRINT IT USING PUTCHAR: ");
		chr = GETCHAR();
		if (chr == '\n')
		{
			chr = GETCHAR();
		}
		if ((chr != '\r') && (chr != EOF))
		{
			PUTCHAR(toupper(chr));
		}
		PRINTF("\r\n");
	}

	return 0;
}
