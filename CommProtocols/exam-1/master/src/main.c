/**
 * @file main.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a program for ESP32C6 to get a LED state from terminal and then send it to MCXN236
 via I2C to change state of the onboard LEDs on MCXN236 and get a confimation response from MCXN236
 (ESP32C6 is the I2C master and MCXN236 is the I2C slave).

    If the user input is not valid, ESP32 shall print => invalid to the terminal.

    If the command is confirmed, ESP32 shall print => done to the terminal.

    If the command isn’t confirmed, ESP32 shall print => failed to the terminal.
 * @version 0.1
 * @date 2026-05-21
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <bootloader_random.h>
#include <ctype.h>
#include <driver/gpio.h>
#include <driver/i2c_master.h>
#include <esp_log.h>
#include <esp_random.h>
#include <esp_task_wdt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MASTER I2C_NUM_0
#define PIN_SDA GPIO_NUM_6 // GPIO for SDA
#define PIN_SCL GPIO_NUM_7 // GPIO for SCL
#define CLOCK_RATE 400000  // Communication Speed
#define SLAVE_ADDRESS 0x55 // Slave Address
#define MSG_SIZE 8         // Buffer Length

static const char *TAG = "I2C_MASTER";

enum
{
    OFF,
    RED,
    GREEN,
    BLUE
};
enum
{
    SUCCESS,
    FAIL
};

static size_t get_string(char *string, size_t length)
{
    char chr = 0;
    size_t len = 0;
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));

    while (len < length)
    {
        chr = getchar();
        if (chr == '\n')
        {
            break;
        }
        else if ((chr >= 'a') && (chr <= 'z'))
        {
            putchar(chr);
            string[len] = toupper(chr);
            len++;
        }
        else
        {
            ;
        }
    }
    ESP_ERROR_CHECK(esp_task_wdt_add(xTaskGetIdleTaskHandle()));
    string[len] = '\0';

    return len;
}

void app_main(void)
{
    i2c_master_bus_handle_t bus_handle;
    i2c_master_bus_config_t bus_config = {
        .i2c_port = MASTER,
        .sda_io_num = PIN_SDA,
        .scl_io_num = PIN_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

    i2c_master_dev_handle_t dev_handle;
    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = SLAVE_ADDRESS,
        .scl_speed_hz = CLOCK_RATE,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_config, &dev_handle));
    ESP_LOGI(TAG, "I2C Master initialized successfully");

    char command[MSG_SIZE + 1] = {0};
    while (1)
    {
        // Clear the input buffer
        while (EOF != getchar())
        {
            ;
        }

        printf("\nEnter the LED state (off, red, green, blue): ");
        size_t length = get_string(command, MSG_SIZE);
        if (command[0] == '\n' || command[0] == '\0')
        {
            usleep(1000); // prevents multiple menu prints
            continue;
        }

        size_t byte = 255;
        const char *off = "OFF";
        const char *red = "RED";
        const char *green = "GREEN";
        const char *blue = "BLUE";

        if (0 == strcmp(command, off))
        {
            byte = OFF;
        }
        else if (0 == strcmp(command, red))
        {
            byte = RED;
        }
        else if (0 == strcmp(command, green))
        {
            byte = GREEN;
        }
        else if (0 == strcmp(command, blue))
        {
            byte = BLUE;
        }

        if (length > 0 && (byte == OFF || byte == RED || byte == GREEN || byte == BLUE))
        {
            uint8_t buffer[1];
            buffer[0] = byte;

            // printf("Sending: %d", buffer[0]);
            if (ESP_OK == i2c_master_transmit(dev_handle, buffer, 1, -1))
            {
                // printf("%s\nReceived: ", buffer[0]);
                if (ESP_OK == i2c_master_receive(dev_handle, buffer, 1, -1))
                {
                    printf("%s", buffer[0] == SUCCESS ? " <= done" : " <= failed");
                }
                else
                {
                    printf("Failed");
                }
            }
            else
            {
                printf("Failed");
            }
        }
        else
        {
            printf(" <= invalid");
            usleep(1000); // prevents multiple menu prints
        }
    }
}