#include "random.h"
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/pm/device.h>
#include <zephyr/drivers/adc.h>

bool random_init(void)
{
    bool status = false;

    const struct device *adc_dev = DEVICE_DT_GET(DT_NODELABEL(lpadc0));
    if (device_is_ready(adc_dev))
    {
        const int ADC_RESOLUTION = 12;
        const int ADC_CHANNEL = 3; /* ARD_A0, P4_6, ADC0_A3 */

        struct adc_channel_cfg channel_cfg = {
            .gain = ADC_GAIN_1,
            .reference = ADC_REF_EXTERNAL0,
            .acquisition_time = ADC_ACQ_TIME_DEFAULT,
            .channel_id = ADC_CHANNEL,
            .input_positive = ADC_CHANNEL,
        };

        if (0 == adc_channel_setup(adc_dev, &channel_cfg))
        {
            uint32_t value = 0;
            struct adc_sequence sequence = {
                .channels = BIT(ADC_CHANNEL),
                .buffer = &value,
                .buffer_size = sizeof(value),
                .resolution = ADC_RESOLUTION,
                .oversampling = 4,
                .calibrate = true,
            };

            status = true;
            int samples = 0;
            while (samples == 0)
            {
                if (0 != adc_read(adc_dev, &sequence))
                {
                    status = false;
                    break;
                }
                samples = value;
            }

            if (status)
            {
                uint32_t seed = 0;
                for (int i = 0; i < samples; i++)
                {
                    if (0 != adc_read(adc_dev, &sequence))
                    {
                        status = false;
                        break;
                    }
                    seed += value;
                }

                srand(seed);
            }
        }

        pm_device_action_run(adc_dev, PM_DEVICE_ACTION_TURN_OFF);
    }

    return status;
}

uint32_t random_get(void)
{
    return rand();
}