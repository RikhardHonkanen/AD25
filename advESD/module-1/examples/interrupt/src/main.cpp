#include <Arduino.h>

#define BUTTON_PIN 33

static uint32_t prev_count = 0;
static volatile uint32_t count = 0;

void button_isr(void)
{
    noInterrupts(); // Disbale interrupts to prevent nested interrupts

    static uint32_t prev_ms = 0;

    uint32_t now = millis();

    if (now - prev_ms > 50)
    {
        count++;
        prev_ms = now;
    }

    interrupts(); // Enable interrupts
}

void setup()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    Serial.begin(9600);
    delay(2000);

    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_isr, FALLING);
}

void loop()
{
    if (prev_count != count)
    {
        Serial.print("Count = ");
        Serial.println(count);
        prev_count = count;
    }

    if (count % 100 == 0)
    {
        noInterrupts();
        count = 0; // A protected shared varaible
        interrupts();
    }
}
