/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of generating two PWM independent signals using the PWM0 module. Connect the LEDs to P3_0 and P3_1.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#pwm-pulse-width-modulator
 *
 * @version 0.1
 * @date 2025-10-14
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "board.h"
#include <assert.h>
#include "fsl_pwm.h"
#include "pin_mux.h"

#define PWM_FREQUENCY 5000UL

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Pwm0);
    CLOCK_EnableClock(kCLOCK_Pwm0_Sm0);

    pwm_config_t pwm_config;
    PWM_GetDefaultConfig(&pwm_config);
    pwm_config.reloadLogic = kPWM_ReloadPwmFullCycle;

    assert(kStatus_Success == PWM_Init(PWM0, kPWM_Module_0, &pwm_config));

    pwm_fault_param_t fault_config;
    PWM_FaultDefaultConfig(&fault_config);
    fault_config.faultLevel = true;

    PWM_SetupFaults(PWM0, kPWM_Fault_0, &fault_config);
    PWM_SetupFaults(PWM0, kPWM_Fault_1, &fault_config);
    PWM_SetupFaults(PWM0, kPWM_Fault_2, &fault_config);
    PWM_SetupFaults(PWM0, kPWM_Fault_3, &fault_config);

    pwm_signal_param_t pwm_param[2];
    uint32_t pwm_frequency = PWM_FREQUENCY;
    uint32_t pwm_src_clk_freq = CLOCK_GetFreq(kCLOCK_BusClk);

    pwm_param[0].pwmChannel = kPWM_PwmA;
    pwm_param[0].level = kPWM_HighTrue;
    pwm_param[0].dutyCyclePercent = 0;
    pwm_param[0].faultState = kPWM_PwmFaultState0;
    pwm_param[0].pwmchannelenable = true;

    pwm_param[1].pwmChannel = kPWM_PwmB;
    pwm_param[1].level = kPWM_HighTrue;
    pwm_param[1].dutyCyclePercent = 0;
    pwm_param[1].faultState = kPWM_PwmFaultState0;
    pwm_param[1].pwmchannelenable = true;

    // Edge-Aligned or Center-Aligned? Look at: https://i.stack.imgur.com/JZFJQ.png
    assert(kStatus_Success == PWM_SetupPwm(PWM0, kPWM_Module_0, pwm_param, 2, kPWM_EdgeAligned, pwm_frequency, pwm_src_clk_freq));

    PWM_SetPwmLdok(PWM0, kPWM_Control_Module_0, true);
    PWM_StartTimer(PWM0, kPWM_Control_Module_0);

    int8_t step = 1;
    uint8_t duty = step;

    while (1)
    {
        if ((duty == 100) || (duty == 0))
        {
            step *= -1;
        }
        duty += step;

        SDK_DelayAtLeastUs(25000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        PWM_UpdatePwmDutycycle(PWM0, kPWM_Module_0, kPWM_PwmA, kPWM_EdgeAligned, duty);
        PWM_UpdatePwmDutycycle(PWM0, kPWM_Module_0, kPWM_PwmB, kPWM_EdgeAligned, duty);

        PWM_SetPwmLdok(PWM0, kPWM_Control_Module_0, true);
    }

    return 0;
}
