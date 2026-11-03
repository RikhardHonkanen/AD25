/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of an analog input using interrupt and LPADC0 channel A3.
 * 	      Connect TMP36 to the following pins:
 *           Vs --> VDDA
 *           GND --> GND
 *           Vout --> P4_6 (J4 pin 2)
 *
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

#define ADC_CHANNEL 3U                // The ADC channel is ADC0_A3 which is connected to P4_6(J4:2)
#define ADC_COMMAND_ID 1U             // The command ID
#define ADC_TRIGGER_ID 0U             // The trigger ID
#define ADC_VOLTAGE_REFERENCE 3300.0f // The reference voltage in millivolts

#define TMP36_OFFSET 500    // The sensor offset
#define TMP36_MV_CELSIUS 10 // Every 10 mv is a celcius degree

static float temperature;
static volatile bool conversion_done = false;

void ADC0_IRQHandler(void)
{
    lpadc_conv_result_t result;

    if (LPADC_GetConvResult(ADC0, &result, 0U))
    {
        float millivolts = result.convValue * (ADC_VOLTAGE_REFERENCE / ((1 << ADC_RESOLUTION) - 1));

        temperature = ((millivolts - TMP36_OFFSET) / TMP36_MV_CELSIUS);

        conversion_done = true;
    }
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Enable VREF in active mode
    SPC_EnableActiveModeAnalogModules(SPC0, kSPC_controlVref);

    lpadc_config_t adc_config;
    lpadc_conv_trigger_config_t trigger_config;
    lpadc_conv_command_config_t command_config;

    LPADC_GetDefaultConfig(&adc_config);
    adc_config.enableAnalogPreliminary = true;
    adc_config.powerLevelMode = kLPADC_PowerLevelAlt4;
    adc_config.referenceVoltageSource = kLPADC_ReferenceVoltageAlt3;
    adc_config.FIFO0Watermark = 0;
    LPADC_Init(ADC0, &adc_config);

    // Do offset calibration (offsets for channels A and B)
    assert(kStatus_Success == LPADC_DoOffsetCalibration(ADC0));

    // Do auto calibration
    LPADC_DoAutoCalibration(ADC0);

    // Configure the conversion command slot which can be triggered.
    // A conversion command defines how the ADC should sample and convert an analog input.
    // Each command independently makes a channel and conversion type selection.
    LPADC_GetDefaultConvCommandConfig(&command_config);
    command_config.channelNumber = ADC_CHANNEL;
    command_config.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    command_config.sampleTimeMode = kLPADC_SampleTimeADCK131;
    command_config.hardwareAverageMode = kLPADC_HardwareAverageCount128;
    LPADC_SetConvCommandConfig(ADC0, ADC_COMMAND_ID, &command_config);

    // Set trigger configuration. There are 4 trigger sources
    // Each trigger can be software or hardware generated
    LPADC_GetDefaultConvTriggerConfig(&trigger_config);
    trigger_config.targetCommandId = ADC_COMMAND_ID;
    trigger_config.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(ADC0, ADC_TRIGGER_ID, &trigger_config);

    LPADC_EnableInterrupts(ADC0, kLPADC_FIFO0WatermarkInterruptEnable);
    NVIC_EnableIRQ(ADC0_IRQn);

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
