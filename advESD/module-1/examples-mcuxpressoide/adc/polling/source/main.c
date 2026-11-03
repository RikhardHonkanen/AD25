/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of an analog input using LPADC0 channel A3.
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
#include "fsl_port.h"
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

static void ADC_Init(void)
{
    CLOCK_EnableClock(kCLOCK_Port4); // Enable the port clock

    // Attach FRO HF to ADC0
    CLOCK_SetClkDiv(kCLOCK_DivAdc0Clk, 1U);
    CLOCK_AttachClk(kFRO_HF_to_ADC0);

    port_pin_config_t pin_config;
    pin_config.pullSelect = kPORT_PullDisable;
    pin_config.pullValueSelect = kPORT_LowPullResistor;
    pin_config.slewRate = kPORT_FastSlewRate;
    pin_config.passiveFilterEnable = kPORT_PassiveFilterDisable;
    pin_config.openDrainEnable = kPORT_OpenDrainDisable;
    pin_config.driveStrength = kPORT_LowDriveStrength;
    pin_config.mux = kPORT_MuxAlt0; // Configure as ADC0_A3
    pin_config.inputBuffer = kPORT_InputBufferDisable;
    pin_config.invertInput = kPORT_InputNormal;
    pin_config.lockRegister = kPORT_LockRegister;

    PORT_SetPinConfig(PORT4, 6U, &pin_config); // PORT4_6 (J4:2) is configured as ADC0_A3

    // Enable VREF in active mode
    SPC_EnableActiveModeAnalogModules(SPC0, kSPC_controlVref);

    lpadc_config_t adc_config;
    lpadc_conv_trigger_config_t trigger_config;
    lpadc_conv_command_config_t command_config;

    LPADC_GetDefaultConfig(&adc_config);
    adc_config.enableAnalogPreliminary = true;
    adc_config.powerLevelMode = kLPADC_PowerLevelAlt4;
    adc_config.referenceVoltageSource = kLPADC_ReferenceVoltageAlt3; // Use VREF as the reference voltage
    adc_config.conversionAverageMode = kLPADC_ConversionAverage128;
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
    LPADC_SetConvCommandConfig(ADC0, ADC_COMMAND_ID, &command_config);

    // Set trigger configuration. There are 4 trigger sources
    // Each trigger can be software or hardware generated
    LPADC_GetDefaultConvTriggerConfig(&trigger_config);
    trigger_config.targetCommandId = ADC_COMMAND_ID;
    trigger_config.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(ADC0, ADC_TRIGGER_ID, &trigger_config);
}

static float read_temperature(void)
{
    lpadc_conv_result_t result;

    LPADC_DoSoftwareTrigger(ADC0, 1U << ADC_TRIGGER_ID);

    while (!LPADC_GetConvResult(ADC0, &result, 0U))
    {
        ;
    }

    float millivolts = result.convValue * (ADC_VOLTAGE_REFERENCE / ((1 << ADC_RESOLUTION) - 1));

    return ((millivolts - TMP36_OFFSET) / TMP36_MV_CELSIUS);
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
        SDK_DelayAtLeastUs(500000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        printf("\r\nTemperature: %.2f °C", read_temperature());
    }

    return 0;
}
