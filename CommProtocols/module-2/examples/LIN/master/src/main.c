/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of a LIN master using UART.
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/stable/esp32c6/api-reference/peripherals/uart.html
 *
 * @version 0.1
 * @date 2025-04-01
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <driver/gpio.h>
#include <driver/uart.h>

#define UART UART_NUM_1                  // Using UART1
#define TX_PIN GPIO_NUM_5                // Define TX pin
#define RX_PIN GPIO_NUM_4                // Define RX pin
#define BUF_SIZE (2 * SOC_UART_FIFO_LEN) // Buffer size shall be greater than SOC_UART_FIFO_LEN

#define DLEN(x) ((x <= 31) ? 2 : ((x <= 47) ? 4 : 8))
#define PID(x) (x | ((((x >> 0) ^ (x >> 1) ^ (x >> 2) ^ (x >> 4)) & 1) << 6) | ((!(((x >> 1) ^ (x >> 3) ^ (x >> 4) ^ (x >> 5)) & 1)) << 7))

#define MSGID 50
#define SPEED 16384                                                                          // 16 kbps
#define BIT_TIME_US (((1000000 % SPEED) == 0) ? (1000000 / SPEED) : ((1000000 / SPEED) + 1)) // Bit time in usec

typedef struct
{
    uint8_t pid;
    uint8_t len;
    uint8_t data[8];
} message_t;

static uint8_t buffer[BUF_SIZE];
static uart_config_t config = {
    .baud_rate = SPEED,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_DEFAULT,
};

static void write_break(void)
{
    ESP_ERROR_CHECK(gpio_reset_pin(TX_PIN));
    ESP_ERROR_CHECK(gpio_set_direction(TX_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(TX_PIN, 0));

    usleep(13 * BIT_TIME_US);

    uart_driver_delete(UART);
    ESP_ERROR_CHECK(uart_driver_install(UART, BUF_SIZE, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART, &config));
    ESP_ERROR_CHECK(uart_set_pin(UART, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    usleep(BIT_TIME_US);
}

static bool make_request(message_t *msg)
{
    bool status = false;

    buffer[0] = 0x55;
    buffer[1] = msg->pid;
    if (2 == uart_write_bytes(UART, buffer, 2))
    {
        if ((msg->len + 1) == uart_read_bytes(UART, buffer, msg->len + 1, pdMS_TO_TICKS(25)))
        {
            uint16_t checksum = msg->pid;
            for (size_t i = 0; i < msg->len; i++)
            {
                msg->data[i] = buffer[i];

                checksum += buffer[i];
                if (checksum > 255)
                {
                    checksum -= 255;
                }
            }
            checksum += buffer[msg->len];

            status = (checksum == 255);
        }
    }

    return status;
}

void app_main()
{
    // Install driver and configure UART
    ESP_ERROR_CHECK(uart_driver_install(UART, BUF_SIZE, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART, &config));
    ESP_ERROR_CHECK(uart_set_pin(UART, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    message_t msg = {.pid = PID(MSGID), .len = DLEN(MSGID)};

    while (1)
    {
        write_break();

        if (make_request(&msg))
        {
            printf("Response: %.*s\n", msg.len, msg.data);
        }
        else
        {
            printf("Failed to make a request\n");
        }

        sleep(1);
    }
}
