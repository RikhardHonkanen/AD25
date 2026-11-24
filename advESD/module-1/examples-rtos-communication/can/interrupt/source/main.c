/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of CAN0 configured in loopback mode using interrupt
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#flexcan-flex-controller-area-network-driver
 *
 * @version 0.1
 * @date 2025-10-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <stdio.h>
#include "board.h"
#include <stdlib.h>
#include "pin_mux.h"
#include "fsl_flexcan.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define MESSAGE_DLC (8)
#define RX_MESSAGE_BUFFER_NUM (0)
#define TX_MESSAGE_BUFFER_NUM (1)

static flexcan_frame_t rx_frame;
static volatile bool rx_completed = false;

void CAN0_IRQHandler(void)
{
    if (0U != FLEXCAN_GetMbStatusFlags(CAN0, (uint64_t)1U << RX_MESSAGE_BUFFER_NUM))
    {
        FLEXCAN_ClearMbStatusFlags(CAN0, (uint64_t)1U << RX_MESSAGE_BUFFER_NUM);
        (void)FLEXCAN_ReadRxMb(CAN0, RX_MESSAGE_BUFFER_NUM, &rx_frame);
        rx_completed = true;
    }
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();

    printf("\r\nAn example of FlexCAN in loopback mode using interrupt\r\n\r\n");

    flexcan_config_t flexcan_config;
    FLEXCAN_GetDefaultConfig(&flexcan_config);
    flexcan_config.enableLoopBack = true;
    flexcan_config.bitRate = 1000000U;

    flexcan_timing_config_t timing_config = {0};
    uint32_t clock_freq = CLOCK_GetFlexcanClkFreq(1U);

    // Calculate and update the improved timing configuration
    FLEXCAN_CalculateImprovedTimingValues(CAN0, flexcan_config.bitRate, clock_freq, &timing_config);
    memcpy(&(flexcan_config.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));

    FLEXCAN_Init(CAN0, &flexcan_config, clock_freq);

    // Setup Rx Message Buffer
    flexcan_rx_mb_config_t mb_config;
    mb_config.format = kFLEXCAN_FrameFormatStandard;
    mb_config.type = kFLEXCAN_FrameTypeData;
    mb_config.id = FLEXCAN_ID_STD(0x123);

    FLEXCAN_SetRxMbConfig(CAN0, RX_MESSAGE_BUFFER_NUM, &mb_config, true);

    FLEXCAN_SetTxMbConfig(CAN0, TX_MESSAGE_BUFFER_NUM, true);

    // Enable Rx Message Buffer interrupt
    FLEXCAN_EnableMbInterrupts(CAN0, (uint64_t)1U << RX_MESSAGE_BUFFER_NUM);
    (void)NVIC_EnableIRQ(CAN0_IRQn);

    flexcan_frame_t tx_frame = {
        .format = (uint8_t)kFLEXCAN_FrameFormatStandard,
        .type = (uint8_t)kFLEXCAN_FrameTypeData,
        .id = FLEXCAN_ID_STD(0x123),
        .length = (uint8_t)MESSAGE_DLC};

    uint8_t buffer[MESSAGE_DLC + 1] = {0};

    while (1)
    {
        // Generate a random uppercase string
        for (int i = 0; i < MESSAGE_DLC; i++)
        {
            buffer[i] = 'A' + (rand() % 26);
        }
        printf("      Sent message to MB%d: %s\r\n", TX_MESSAGE_BUFFER_NUM, buffer);

        tx_frame.dataByte0 = buffer[0];
        tx_frame.dataByte1 = buffer[1];
        tx_frame.dataByte2 = buffer[2];
        tx_frame.dataByte3 = buffer[3];
        tx_frame.dataByte4 = buffer[4];
        tx_frame.dataByte5 = buffer[5];
        tx_frame.dataByte6 = buffer[6];
        tx_frame.dataByte7 = buffer[7];

        rx_completed = false;

        // Send data through Tx Message Buffer
        (void)FLEXCAN_TransferSendBlocking(CAN0, TX_MESSAGE_BUFFER_NUM, &tx_frame);

        // Waiting for Message receive finish
        while (!rx_completed)
        {
            ;
        }

        // Copy the received message to the buffer
        memset(buffer, 0, sizeof(buffer));
        buffer[0] = rx_frame.dataByte0;
        buffer[1] = rx_frame.dataByte1;
        buffer[2] = rx_frame.dataByte2;
        buffer[3] = rx_frame.dataByte3;
        buffer[4] = rx_frame.dataByte4;
        buffer[5] = rx_frame.dataByte5;
        buffer[6] = rx_frame.dataByte6;
        buffer[7] = rx_frame.dataByte7;

        printf("Received message from MB%d: %s\r\n\r\n", RX_MESSAGE_BUFFER_NUM, buffer);

        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }

    return 0;
}
