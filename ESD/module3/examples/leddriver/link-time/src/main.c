#include <unistd.h>
#include <assert.h>
#include "led_driver.h"

#define LED_PIN 4

void app_main(void)
{
    assert(led_driver_init(LED_PIN));

    int state = 0;

    while (1)
    {
        state = !state;

        assert(led_driver_set_state(state));

        sleep(1); // 1s delay
    }
}
