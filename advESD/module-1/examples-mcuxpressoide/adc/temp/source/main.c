/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of read the internal temperature sensor using ADC0 channel 26 interrupt.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#lpadc-12-bit-sar-analog-to-digital-converter-driver
 *
 * @version 0.1
 * @date 2025-10-17
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include "board.h"
#include <assert.h>
#include "pin_mux.h"
#include "fsl_spc.h"
#include "fsl_lpadc.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

// If the chosen conversion resolution is kLPADC_ConversionResolutionHigh, the ADC resolution is 16 bits and
// if it is kLPADC_ConversionResolutionStandard, the ADC resolution is 16 bits and the result shall be shifted 3 times to right
#define ADC_RESOLUTION 16U

#define ADC_TEMP_CHANNEL 26U // Look at the manual reference; p. 1040
#define ADC_COMMAND_ID 1U    // The command ID
#define ADC_TRIGGER_ID 0U    // The trigger ID

static float temperature;
static volatile bool conversion_done = false;

void ADC0_IRQHandler(void)
{
    lpadc_conv_result_t result;

    // Read the sensor 2 times
    if (LPADC_GetConvResult(ADC0, &result, 0U))
    {
        uint16_t Vbe1 = result.convValue;

        if (LPADC_GetConvResult(ADC0, &result, 0U))
        {
            uint16_t Vbe8 = result.convValue;

            // Look at the reference manual; p. 1048
            // Temp = A*[alpha*(Vbe8 - Vbe1)/(Vbe8 + alpha*(Vbe8 - Vbe1))] - B
            temperature = FSL_FEATURE_LPADC_TEMP_PARAMETER_A * (FSL_FEATURE_LPADC_TEMP_PARAMETER_ALPHA * ((float)Vbe8 - (float)Vbe1) /
                                                                ((float)Vbe8 + FSL_FEATURE_LPADC_TEMP_PARAMETER_ALPHA * ((float)Vbe8 - (float)Vbe1))) -
                          FSL_FEATURE_LPADC_TEMP_PARAMETER_B;

            conversion_done = true;
        }
    }
}

static void ADC_Init(void)
{
    CLOCK_EnableClock(kCLOCK_Port4); // Enable the port clock

    // Attach FRO HF to ADC0
    CLOCK_SetClkDiv(kCLOCK_DivAdc0Clk, 1U);
    CLOCK_AttachClk(kFRO_HF_to_ADC0);

    // Enable VREF in active mode
    SPC_EnableActiveModeAnalogModules(SPC0, kSPC_controlVref);

    lpadc_config_t adc_config;
    lpadc_conv_trigger_config_t trigger_config;
    lpadc_conv_command_config_t command_config;

    LPADC_GetDefaultConfig(&adc_config);
    adc_config.enableAnalogPreliminary = true;
    adc_config.powerLevelMode = kLPADC_PowerLevelAlt4;
    adc_config.referenceVoltageSource = kLPADC_ReferenceVoltageAlt3;
    adc_config.conversionAverageMode = kLPADC_ConversionAverage128;
    adc_config.FIFO0Watermark = FSL_FEATURE_LPADC_TEMP_SENS_BUFFER_SIZE - 1U;
    adc_config.FIFO1Watermark = 0U;
    LPADC_Init(ADC0, &adc_config);

    // Do offset calibration (offsets for channels A and B)
    assert(kStatus_Success == LPADC_DoOffsetCalibration(ADC0));

    // Do auto calibration
    LPADC_DoAutoCalibration(ADC0);

    // Configure the conversion command slot which can be triggered.
    // A conversion command defines how the ADC should sample and convert an analog input.
    // Each command independently makes a channel and conversion type selection.
    LPADC_GetDefaultConvCommandConfig(&command_config);
    command_config.channelNumber = ADC_TEMP_CHANNEL;
    command_config.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    command_config.sampleTimeMode = kLPADC_SampleTimeADCK131;
    command_config.hardwareAverageMode = kLPADC_HardwareAverageCount128;
    command_config.sampleChannelMode = kLPADC_SampleChannelDiffBothSide;
    command_config.loopCount = FSL_FEATURE_LPADC_TEMP_SENS_BUFFER_SIZE - 1U;

    LPADC_SetConvCommandConfig(ADC0, ADC_COMMAND_ID, &command_config);

    // Set trigger configuration. There are 4 trigger sources
    // Each trigger can be software or hardware generated
    LPADC_GetDefaultConvTriggerConfig(&trigger_config);
    trigger_config.targetCommandId = ADC_COMMAND_ID;
    trigger_config.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(ADC0, ADC_TRIGGER_ID, &trigger_config);

    LPADC_EnableInterrupts(ADC0, kLPADC_FIFO0WatermarkInterruptEnable);
    NVIC_EnableIRQ(ADC0_IRQn);
}

int main(void)
{
    // Init board hardware
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    ADC_Init(); // Initialize the ADC

    while (1)
    {
        conversion_done = false;

        LPADC_DoSoftwareTrigger(ADC0, 1U << ADC_TRIGGER_ID);

        while (!conversion_done)
        {
            __WFI();
        }

        printf("\r\nTemperature: %.2f °C", temperature);

        SDK_DelayAtLeastUs(500000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }

    return 0;
}
