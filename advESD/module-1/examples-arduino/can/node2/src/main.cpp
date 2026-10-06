#include <FlexCAN.h>
#include <Arduino.h>

#define MESSAGES (3U)
#define BIT_RATE (250000U)

static uint32_t tx_millis = 0;
static uint32_t rx_millis = 0;
static CAN_message_t messages[MESSAGES] = {
    {.id = 0x190, .timestamp = 0, .flags = {.extended = 0, .remote = 0}, .len = 8, .buf = {0}},
    {.id = 0x200, .timestamp = 0, .flags = {.extended = 0, .remote = 0}, .len = 8, .buf = {0}},
    {.id = 0x210, .timestamp = 0, .flags = {.extended = 0, .remote = 0}, .len = 8, .buf = {0}},
};

void setup()
{
    Serial.begin(9600);
    delay(2000);

    // Fill the buffers
    memcpy(messages[0].buf, "ABCDEFGH", messages[0].len);
    memcpy(messages[1].buf, "IJKLMNOP", messages[1].len);
    memcpy(messages[2].buf, "QRSTUVWX", messages[2].len);

    Serial.println("\nStarting Can0 ...");
    Can0.begin(BIT_RATE);

    Serial.println("Setting the filters...");

    // Set up a blockin filter/mask for all the RX mailboxes
    CAN_filter_t filter = {.id = 0x7FF, .flags = {.extended = 1, .remote = 0}}; // The blockin filter

    for (int i = 0; i < NUM_MAILBOXES; ++i)
    {
        Can0.setFilter(filter, i);
        Can0.setMask(0x000, i);
    }

    // Set up the filter/mask for the first mailbox in order to let the
    // messages with ids 0x100 and 0x110 in

    // 0x100  = 001 0000 0000
    // 0x110  = 001 0001 0000
    // ----------------------
    // Id     = 001 0000 0000 = 0x100
    // Mask   = 111 1110 1111 = 0x7EF

    filter.id = 0x100;
    filter.flags.extended = 0;
    Can0.setFilter(filter, 0);
    Can0.setMask(0x7EF << 18, 0); // The 3 msb bits are reserved
}

void loop()
{
    uint32_t now = millis();

    if (now - tx_millis >= 250U)
    {
        tx_millis = now;
        for (uint8_t i = 0; i < MESSAGES; i++)
        {
            Can0.write(messages[i]);
        }
    }

    if (now - rx_millis >= 500)
    {
        rx_millis = now;

        if (Can0.available())
        {
            CAN_message_t message;
            while (Can0.read(message))
            {
                Serial.printf("RX - ID: 0x%03X - [%d] - ", message.id, message.len);
                for (int i = 0; i < message.len; ++i)
                {
                    Serial.printf("%c ", message.buf[i]);
                }
                Serial.print("\n");
            }
            Serial.print("\n");
        }
    }
}
