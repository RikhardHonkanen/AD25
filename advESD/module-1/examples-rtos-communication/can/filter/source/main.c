/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of CAN0 configured in loopback mode using filtering
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
#include "pin_mux.h"
#include "fsl_flexcan.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define MESSAGE_DLC (8)
#define RX_MESSAGE_BUFFER_NUM (0)
#define TX_MESSAGE_BUFFER_NUM (1)

void CAN0_IRQHandler(void)
{
    if (0U != FLEXCAN_GetMbStatusFlags(CAN0, (uint64_t)1U << RX_MESSAGE_BUFFER_NUM))
    {
        flexcan_frame_t frame;
        FLEXCAN_ClearMbStatusFlags(CAN0, (uint64_t)1U << RX_MESSAGE_BUFFER_NUM);
        (void)FLEXCAN_ReadRxMb(CAN0, RX_MESSAGE_BUFFER_NUM, &frame);
        printf("[0x%0X]: %c%c%c%c%c%c%c%c\r\n", frame.id >> 18,
               frame.dataByte0, frame.dataByte1, frame.dataByte2, frame.dataByte3,
               frame.dataByte4, frame.dataByte5, frame.dataByte6, frame.dataByte7);
    }
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();

    printf("\r\nAn example of FlexCAN in loopback mode using filtering\r\n\r\n");

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

    // 0x101 = 001 0000 0001
    // 0x111 = 001 0001 0001
    // ----------------------
    //  Mask = 111 1110 1111
    //    id = 001 0000 0001
    mb_config.id = FLEXCAN_ID_STD(0x101);

    FLEXCAN_SetRxMbGlobalMask(CAN0, 0x7EF << 18);
    FLEXCAN_SetRxMbConfig(CAN0, RX_MESSAGE_BUFFER_NUM, &mb_config, true);

    FLEXCAN_SetTxMbConfig(CAN0, TX_MESSAGE_BUFFER_NUM, true);

    // Enable Rx Message Buffer interrupt
    FLEXCAN_EnableMbInterrupts(CAN0, (uint64_t)1U << RX_MESSAGE_BUFFER_NUM);
    (void)EnableIRQ(CAN0_IRQn);

    flexcan_frame_t messages[] = {
        {
            .format = kFLEXCAN_FrameFormatStandard,
            .type = kFLEXCAN_FrameTypeData,
            .id = FLEXCAN_ID_STD(0x100),
            .dataByte0 = 'A',
            .dataByte1 = 'A',
            .dataByte2 = 'A',
            .dataByte3 = 'A',
            .dataByte4 = 'A',
            .dataByte5 = 'A',
            .dataByte6 = 'A',
            .dataByte7 = 'A',
            .length = MESSAGE_DLC,
        },
        {
            .format = kFLEXCAN_FrameFormatStandard,
            .type = kFLEXCAN_FrameTypeData,
            .id = FLEXCAN_ID_STD(0x101),
            .dataByte0 = 'B',
            .dataByte1 = 'B',
            .dataByte2 = 'B',
            .dataByte3 = 'B',
            .dataByte4 = 'B',
            .dataByte5 = 'B',
            .dataByte6 = 'B',
            .dataByte7 = 'B',
            .length = MESSAGE_DLC,
        },
        {
            .format = kFLEXCAN_FrameFormatStandard,
            .type = kFLEXCAN_FrameTypeData,
            .id = FLEXCAN_ID_STD(0x111),
            .dataByte0 = 'C',
            .dataByte1 = 'C',
            .dataByte2 = 'C',
            .dataByte3 = 'C',
            .dataByte4 = 'C',
            .dataByte5 = 'C',
            .dataByte6 = 'C',
            .dataByte7 = 'C',
            .length = MESSAGE_DLC,
        },
    };

    while (1)
    {
        printf("\r\n");

        for (int i = 0; i < sizeof(messages) / sizeof(messages[0]); i++)
        {
            // Send data through Tx Message Buffer
            (void)FLEXCAN_TransferSendBlocking(CAN0, TX_MESSAGE_BUFFER_NUM, &messages[i]);
        }

        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }

    return 0;
}
