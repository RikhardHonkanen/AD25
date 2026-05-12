#include <stdio.h>
#include <esp_log.h>
#include <esp_random.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <freertos/FreeRTOS.h>
#include <bootloader_random.h>

#define SPI_HOST SPI2_HOST      // SPI Controller
#define SPI_CS_PIN GPIO_NUM_18  // FSPICS2
#define SPI_CLK_PIN GPIO_NUM_6  // FSPICLK
#define SPI_MISO_PIN GPIO_NUM_7 // FSPID
#define SPI_MOSI_PIN GPIO_NUM_2 // FSPIIQ
#define SPI_CLOCK_RATE 500000   // Clock Rate
#define MSG_SIZE 8              // Buffer Length

static const char *TAG = "SPI_MASTER";

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
    ESP_ERROR_CHECK(spi_bus_initialize(SPI_HOST, &bus_config, SPI_DMA_DISABLED));

    spi_device_handle_t handle;
    spi_device_interface_config_t device_config = {
        .clock_speed_hz = SPI_CLOCK_RATE,
        .clock_source = SPI_CLK_SRC_DEFAULT,
        .mode = 0,
        .spics_io_num = SPI_CS_PIN,
        .queue_size = 2,
    };
    ESP_ERROR_CHECK(spi_bus_add_device(SPI_HOST, &device_config, &handle));

    ESP_LOGI(TAG, "SPI2 Master initialized successfully");

    bootloader_random_enable();
    srand(esp_random());
    bootloader_random_disable();

    uint8_t rxbuf[MSG_SIZE + 1] = {0};
    uint8_t txbuf[MSG_SIZE + 1] = {0};

    spi_transaction_t tx_trans = {.length = MSG_SIZE * CHAR_BIT, .tx_buffer = txbuf};
    spi_transaction_t rx_trans = {.length = MSG_SIZE * CHAR_BIT, .rx_buffer = rxbuf};

    while (1)
    {
        for (int i = 0; i < MSG_SIZE; i++)
        {
            txbuf[i] = 'a' + (rand() % 26);
        }

        ESP_ERROR_CHECK(spi_device_acquire_bus(handle, portMAX_DELAY));

        printf("    Sent: ");
        if (ESP_OK == spi_device_polling_transmit(handle, &tx_trans))
        {
            printf("%s\nReceived: ", txbuf);

            vTaskDelay(pdMS_TO_TICKS(10)); // Wait on the slave to convert the string to uppercase

            if (ESP_OK == spi_device_polling_transmit(handle, &rx_trans))
            {
                printf("%s", rxbuf);
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
        printf("\n\n");

        spi_device_release_bus(handle);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
