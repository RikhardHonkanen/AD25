#include "random.h"
#include <cstdlib>
#include <zephyr/device.h>
#include <zephyr/pm/device.h>
#include <zephyr/drivers/adc.h>

bool random_init(void)
{
    bool status{false};

    const struct device *adc = DEVICE_DT_GET(DT_NODELABEL(lpadc0));

    if (device_is_ready(adc))
    {
        constexpr int ADC_CHANNEL{3}; /* ARD_A0, P4_6, ADC0_A3 */
        constexpr adc_gain ADC_GAIN{ADC_GAIN_1};

        adc_channel_cfg config{
            .gain = ADC_GAIN,
            .reference = ADC_REF_EXTERNAL1,
            .acquisition_time = ADC_ACQ_TIME_DEFAULT,
            .channel_id = ADC_CHANNEL,
            .differential = 0,
            .input_positive = ADC_CHANNEL,
        };

        if (0 == adc_channel_setup(adc, &config))
        {
            uint16_t value{0};
            constexpr int ADC_RESOLUTION{12};

            adc_sequence sequence = {
                .channels = BIT(ADC_CHANNEL),
                .buffer = &value,
                .buffer_size = sizeof(value),
                .resolution = ADC_RESOLUTION,
                .oversampling = 4,
                .calibrate = true,
            };

            size_t i{0};
            int32_t seed{0};
            int8_t *ptr{reinterpret_cast<int8_t *>(&seed)};

            status = true;
            while (i < sizeof(seed))
            {
                if (0 == adc_read(adc, &sequence))
                {
                    ptr[i] = value % 256;
                    if ((i == 0) || (ptr[i] != ptr[i - 1]))
                    {
                        i++;
                    }
                    k_msleep(1);
                }
                else
                {
                    status = false;
                    break;
                }
            }

            if (seed < 0)
            {
                seed *= -1;
            }
            srand(seed);
        }

        pm_device_action_run(adc, PM_DEVICE_ACTION_TURN_OFF);
    }

    return status;
}

void random_get(uint8_t *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        data[i] = rand() % 256;
    }
}