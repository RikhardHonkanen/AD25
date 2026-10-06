#include <Arduino.h>

#define MAX_LEN 16

static char *get_string(void)
{
    size_t i = 1;
    char string[MAX_LEN]{0};

    while (i < MAX_LEN)
    {
        if (Serial.available())
        {
            int chr = Serial.read();

            if ((chr >= 'a') && (chr <= 'z'))
            {
                i++;
                Serial.printf("%c", chr);
                string[i] = chr;
            }
            else if (chr == '\n')
            {
                break;
            }
            else
            {
                ;
            }
        }
    }

    return string;
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
    Serial.clear();

    Serial.printf("Enter a string (max %d characters): ", MAX_LEN);

    char *string{get_string()};

    Serial.println();

    for (size_t i = 0; i < MAX_LEN; i++)
    {
        Serial.printf("%c", string[i]);
    }
}
