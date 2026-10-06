#include <Arduino.h>
#include <TimeLib.h>

typedef struct
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} datetime_t;

/**
 * @brief This function is used to read the command from the user.
 *        The commands can be 'S' for setting date and time of the RTC,
 *        'G' for getting the current date and time of the RTC and display it or
 *        a new line for printing a new line and displaying the menu.
 *
 * @return The command can be 'S', 'G' or new line.
 */
static char read_command(void)
{
    char command = 0;

    while ((command != 'S') && (command != 'G') && (command != '\n'))
    {
        if (Serial.available())
        {
            command = toupper(Serial.read());
        }
    }

    Serial.printf("%c", command);

    return command;
}

/**
 * @brief This function is used to read date and time in the fromat of YYYY-MM-DD hh:mm:ss
 *        from the user and format it in the structure of datetime_t and return it.
 *
 * @return datetime_t The formated date and time in the structure of datetime_t.
 */
static datetime_t read_datetime(void)
{
    Serial.clear();
    Serial.printf("\nEnter the current datetime (YYYY-MM-DD HH:MM:SS): ");

    uint8_t i = 0;
    char text[20] = {0};

    while (i < 19)
    {
        if (Serial.available())
        {
            char temp = Serial.read();
            if (temp == '\n')
            {
                break;
            }
            else if (isdigit(temp) || isspace(temp) || (temp == '-') || (temp == ':'))
            {
                Serial.print(temp);
                text[i] = temp;
                i++;
            }
        }
    }

    datetime_t datetime = {};
    sscanf(text, "%04d-%02d-%02d %02d:%02d:%02d",
           &datetime.year, &datetime.month, &datetime.day,
           &datetime.hour, &datetime.minute, &datetime.second);

    return datetime;
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
}

void loop()
{
    datetime_t datetime;

    Serial.println("S) Set datetime");
    Serial.println("G) Get datetime");
    Serial.print("Enter a command: ");

    switch (read_command())
    {
    case 'S':
        datetime = read_datetime(); // Get date and time from the user
        setTime(datetime.hour, datetime.minute, datetime.second, datetime.day, datetime.month, datetime.year);
        Serial.println("\n");
        break;

    case 'G':
        Serial.print("\nCurrent datetime: ");
        Serial.printf("%04d-%02d-%02d %02d:%02d:%02d\n\n", year(), month(), day(), hour(), minute(), second());
        break;

    case '\n':
        Serial.println();
        break;
    default:
        break;
    }
}
