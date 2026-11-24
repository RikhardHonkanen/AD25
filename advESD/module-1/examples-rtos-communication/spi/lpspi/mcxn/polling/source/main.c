/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of LPSPI2 in master interrupt mode and LPSPI3 in slave transfer polling mode.
          LPSPI2 and LPSPI3 pins on the same board shall be connected together according to README.md
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#lpspi-peripheral-driver
 *
 * @version 0.1
 * @date 2025-10-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_lpspi.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define MSGLEN 8
#define BAUDRATE 1000000

#define SLAVE LPSPI3

#define MASTER LPSPI2
#define MASTER_IRQn LP_FLEXCOMM2_IRQn
#define MASTER_IRQ_HANDLER LP_FLEXCOMM2_IRQHandler
#define MASTER_CLK_FREQ CLOCK_GetLPFlexCommClkFreq(2)

static volatile bool completed;

static void slave_callback(LPSPI_Type *, lpspi_slave_handle_t *, status_t, void *)
{
    completed = true;
}

void MASTER_IRQ_HANDLER(void)
{
    LPSPI_ClearStatusFlags(MASTER, kLPSPI_AllStatusFlag);

    while (0 < LPSPI_GetRxFifoCount(MASTER))
    {
        (void)LPSPI_ReadData(MASTER);
    }
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();

    // Attach FRO 12M to FLEXCOMM2
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    // Attach FRO 12M to FLEXCOMM3
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom3Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM3);

    lpspi_master_config_t master_config;
    LPSPI_MasterGetDefaultConfig(&master_config);
    master_config.baudRate = BAUDRATE;

    LPSPI_MasterInit(MASTER, &master_config, MASTER_CLK_FREQ);
    uint8_t master_fifo_size = LPSPI_GetTxFifoSize(MASTER);

    LPSPI_FlushFifo(MASTER, true, true);
    LPSPI_ClearStatusFlags(MASTER, kLPSPI_AllStatusFlag);
    LPSPI_DisableInterrupts(MASTER, kLPSPI_AllInterruptEnable);
    NVIC_EnableIRQ(MASTER_IRQn);

    lpspi_slave_config_t slave_config;
    LPSPI_SlaveGetDefaultConfig(&slave_config);
    slave_config.whichPcs = kLPSPI_Pcs0;
    LPSPI_SlaveInit(SLAVE, &slave_config);

    lpspi_slave_handle_t handle;
    LPSPI_SlaveTransferCreateHandle(SLAVE, &handle, slave_callback, NULL);

    uint8_t counter;
    uint8_t buffer[MSGLEN + 1] = {0U};
    uint8_t received[MSGLEN + 1] = {0U};
    lpspi_transfer_t xfer = {.dataSize = MSGLEN};

    while (1)
    {
        counter = 0;
        completed = false;
        xfer.txData = NULL;
        xfer.rxData = received;
        assert(kStatus_Success == LPSPI_SlaveTransferNonBlocking(SLAVE, &handle, &xfer));

        LPSPI_EnableInterrupts(MASTER, kLPSPI_RxInterruptEnable);
        while (counter < MSGLEN)
        {
            if (master_fifo_size > LPSPI_GetTxFifoCount(MASTER))
            {
                buffer[counter] = 'a' + (rand() % 26);
                LPSPI_WriteData(MASTER, buffer[counter]);
                counter++;
            }
        }
        while (!completed)
        {
            ;
        }
        LPSPI_DisableInterrupts(MASTER, kLPSPI_RxInterruptEnable);

        PRINTF("Master Write: %s\r\n", buffer);
        PRINTF("  Slave Read: %s\r\n\r\n", received);

        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }

    return 0;
}
