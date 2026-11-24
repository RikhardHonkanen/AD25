/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of FLEXIO as SPI master and LPSPI3 as slave. For the connections look at README.md
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#lpspi-peripheral-driver
 *                 https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#flexio-spi-driver
 *
 * @version 0.1
 * @date 2025-10-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "board.h"
#include <stdlib.h>
#include <assert.h>
#include "pin_mux.h"
#include "fsl_lpspi.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_flexio_spi.h"
#include "fsl_debug_console.h"

#define MSGLEN 8
#define BAUDRATE 1000000

#define MASTER_FLEXIO_SPI FLEXIO0
#define MASTER_FLEXIO_SPI_DIN_PIN 16U
#define MASTER_FLEXIO_SPI_DOUT_PIN 17U
#define MASTER_FLEXIO_SPI_CLK_PIN 18U
#define MASTER_FLEXIO_SPI_PCS_PIN 19U
#define MASTER_FLEXIO_SPI_IRQn FLEXIO_IRQn

#define SLAVE_LPSPI LPSPI3
#define SLAVE_LPSPI_IRQn LP_FLEXCOMM3_IRQn

static volatile bool slave_completed = false;
static volatile bool master_completed = false;

static void master_callback(FLEXIO_SPI_Type *, flexio_spi_master_handle_t *, status_t status, void *)
{
    master_completed = true;
}

static void slave_callback(LPSPI_Type *, lpspi_slave_handle_t *, status_t status, void *)
{
    slave_completed = true;
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Attach FRO HF to FLEXIO
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_FLEXIO);

    // Attach FRO 12M to FLEXCOMM3
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom3Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM3);

    flexio_spi_master_config_t master_config;
    FLEXIO_SPI_MasterGetDefaultConfig(&master_config);
    master_config.baudRate_Bps = BAUDRATE;

    FLEXIO_SPI_Type spi_dev = {
        .flexioBase = MASTER_FLEXIO_SPI,
        .SDOPinIndex = MASTER_FLEXIO_SPI_DOUT_PIN,
        .SDIPinIndex = MASTER_FLEXIO_SPI_DIN_PIN,
        .SCKPinIndex = MASTER_FLEXIO_SPI_CLK_PIN,
        .CSnPinIndex = MASTER_FLEXIO_SPI_PCS_PIN,
        .shifterIndex = {0, 1},
        .timerIndex = {0, 1}};

    FLEXIO_SPI_MasterInit(&spi_dev, &master_config, CLOCK_GetFlexioClkFreq());

    lpspi_slave_config_t slave_config;
    LPSPI_SlaveGetDefaultConfig(&slave_config);
    LPSPI_SlaveInit(SLAVE_LPSPI, &slave_config);

    // Set lpspi slave interrupt priority higher
    NVIC_SetPriority(MASTER_FLEXIO_SPI_IRQn, 1U);
    NVIC_SetPriority(SLAVE_LPSPI_IRQn, 0U);

    lpspi_slave_handle_t slave_handle;
    flexio_spi_master_handle_t master_handle;
    LPSPI_SlaveTransferCreateHandle(SLAVE_LPSPI, &slave_handle, slave_callback, NULL);
    assert(kStatus_Success == FLEXIO_SPI_MasterTransferCreateHandle(&spi_dev, &master_handle, master_callback, NULL));

    uint8_t slave_buffer[MSGLEN + 1] = {0U};
    lpspi_transfer_t slave_xfer = {.txData = NULL, .rxData = slave_buffer, .dataSize = MSGLEN};

    uint8_t master_buffer[MSGLEN + 1] = {0U};
    flexio_spi_transfer_t master_xfer = {.txData = master_buffer, .rxData = NULL, .dataSize = MSGLEN};

    while (1)
    {
        slave_completed = false;
        master_completed = false;

        for (int i = 0; i < MSGLEN; i++)
        {
            master_buffer[i] = 'A' + (rand() % 26);
        }

        assert(kStatus_Success == LPSPI_SlaveTransferNonBlocking(SLAVE_LPSPI, &slave_handle, &slave_xfer));
        assert(kStatus_Success == FLEXIO_SPI_MasterTransferNonBlocking(&spi_dev, &master_handle, &master_xfer));

        while (!(master_completed && slave_completed))
        {
            ;
        }

        PRINTF("Master Write: %s\r\n", master_buffer);
        PRINTF("  Slave Read: %s\r\n\r\n", slave_buffer);

        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }

    return 0;
}
