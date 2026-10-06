#include <math.h>
#include <Arduino.h>

#define NUM_MIN 2
#define NUM_MAX 1000
#define FIRST_PRIME 2
#define STRINGIFY(x) #x
#define LENGTH(x) sizeof(STRINGIFY(x))

static int get_number(void)
{
    size_t i = 1;
    int number{0};

    while (i < LENGTH(NUM_MAX))
    {
        if (Serial.available())
        {
            int chr = Serial.read();

            if ((chr >= '0') && (chr <= '9'))
            {
                i++;
                Serial.printf("%c", chr);
                number = 10 * number + (chr - '0');
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

    return number;
}

static bool isprime(int number)
{
    bool prime = true;

    if (number < FIRST_PRIME)
    {
        prime = false;
    }
    else
    {
        if (number > FIRST_PRIME)
        {
            int counter = FIRST_PRIME;
            int max_number = ceil(sqrt(number));

            while (counter <= max_number)
            {
                if (number % counter == 0)
                {
                    prime = false;
                    break;
                }

                counter++;
            }
        }
    }

    return prime;
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

    Serial.printf("Enter a number in the range of [%d, %d]: ", NUM_MIN, NUM_MAX);

    int number{get_number()};

    Serial.println();

    if ((number >= NUM_MIN) && (number <= NUM_MAX))
    {
        Serial.printf("%d is %sprime!\n\n", number, isprime(number) ? "" : "not ");
    }
}
