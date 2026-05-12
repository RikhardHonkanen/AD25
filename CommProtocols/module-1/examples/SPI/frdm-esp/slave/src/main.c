/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of SPI in slave mode.
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/stable/esp32c6/api-reference/peripherals/spi_slave.html
 *
 * @version 0.1
 * @date 2025-02-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/spi_slave.h>

#define SPI_HOST SPI2_HOST      // SPI Controller
#define SPI_CS_PIN GPIO_NUM_18  // FSPICS2
#define SPI_CLK_PIN GPIO_NUM_6  // FSPICLK
#define SPI_MISO_PIN GPIO_NUM_2 // FSPIIQ
#define SPI_MOSI_PIN GPIO_NUM_7 // FSPID
#define MSG_SIZE 8              // Buffer Length

static const char *TAG = "SPI_SLAVE";

void app_main(void)
{
    spi_bus_config_t bus_config = {
        .miso_io_num = SPI_MISO_PIN,
        .mosi_io_num = SPI_MOSI_PIN,
        .sclk_io_num = SPI_CLK_PIN,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
        .max_transfer_sz = MSG_SIZE,
    };

    // Configuration for the SPI slave interface
    spi_slave_interface_config_t slave_config = {
        .spics_io_num = SPI_CS_PIN,
        .queue_size = 2, // We have 2 transactions
    };

    // Initialize SPI slave interface
    assert(ESP_OK == spi_slave_initialize(SPI_HOST, &bus_config, &slave_config, SPI_DMA_DISABLED));

    ESP_LOGI(TAG, "SPI2 Slave initialized successfully");

    uint8_t rxbuf[MSG_SIZE] = {0};
    uint8_t txbuf[MSG_SIZE] = {0};

    spi_slave_transaction_t tx_trans = {.length = MSG_SIZE * CHAR_BIT, .tx_buffer = txbuf};
    spi_slave_transaction_t rx_trans = {.length = MSG_SIZE * CHAR_BIT, .rx_buffer = rxbuf};

    while (1)
    {
        if (ESP_OK == spi_slave_transmit(SPI_HOST, &rx_trans, portMAX_DELAY))
        {
            for (int i = 0; i < MSG_SIZE; i++)
            {
                txbuf[i] = toupper(rxbuf[i]);
            }

            if (ESP_OK != spi_slave_transmit(SPI_HOST, &tx_trans, portMAX_DELAY))
            {
                printf("Failed to write\n");
            }
        }
        else
        {
            printf("Failed to read\n");
        }
    }
}