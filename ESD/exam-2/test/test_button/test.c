#include "bsp.h"
#include "button.h"
#include <unity.h>

#define BUTTON_PIN 4

void setUp(void) {}
void tearDown(void) {}

void test_button_initialization(void)
{
    bsp_set_mode_status(false);
    TEST_ASSERT_FALSE(button_init(BUTTON_PIN));

    bsp_set_mode_status(true);
    TEST_ASSERT_TRUE(button_init(BUTTON_PIN));

    TEST_ASSERT_EQUAL_INT(BUTTON_PIN, bsp_get_pin_num());
}

void test_button_press_after_5_samples(void)
{
    button_init(BUTTON_PIN);

    // Simulate stable button press
    bsp_set_pin_state(BUTTON_PRESSED);

    for (int i = 0; i < 5; i++)
    {
        button_update_state();
    }

    TEST_ASSERT_EQUAL_INT(BUTTON_PRESSED, button_get_state());
}

void test_button_falling_edge_detected_once(void)
{
    button_init(BUTTON_PIN);

    bsp_set_pin_state(BUTTON_PRESSED);

    for (int i = 0; i < 5; i++)
    {
        button_update_state();
    }

    TEST_ASSERT_TRUE(button_get_falling_edge());
    TEST_ASSERT_FALSE(button_get_falling_edge());
}

void test_button_bounce_does_not_trigger_press(void)
{
    button_init(BUTTON_PIN);

    // Simulate unstable bouncing
    for (int i = 0; i < 4; i++)
    {
        bsp_set_pin_state(BUTTON_PRESSED);
        button_update_state();

        bsp_set_pin_state(BUTTON_RELEASED);
        button_update_state();
    }

    TEST_ASSERT_EQUAL_INT(BUTTON_RELEASED, button_get_state());

    TEST_ASSERT_FALSE(button_get_falling_edge());
}

void test_button_rising_edge_after_release(void)
{
    button_init(BUTTON_PIN);
    bsp_set_pin_state(BUTTON_PRESSED);

    for (int i = 0; i < BUTTON_SAMPLES; i++)
    {
        button_update_state();
    }

    bsp_set_pin_state(BUTTON_RELEASED);
    for (int i = 0; i < BUTTON_SAMPLES; i++)
    {
        button_update_state();
    }

    TEST_ASSERT_EQUAL_INT(BUTTON_RELEASED, button_get_state());
}

void test_button_same_state_no_change(void)
{
    button_init(BUTTON_PIN);

    // Already released by default
    bsp_set_pin_state(BUTTON_RELEASED);

    for (int i = 0; i < BUTTON_SAMPLES; i++)
    {
        button_update_state();
    }

    TEST_ASSERT_FALSE(button_get_falling_edge());
    TEST_ASSERT_EQUAL_INT(BUTTON_RELEASED, button_get_state());
}

#ifdef DEVENV
int main(void)
#else
void app_main(void)
#endif
{
    UNITY_BEGIN();

    RUN_TEST(test_button_initialization);
    RUN_TEST(test_button_press_after_5_samples);
    RUN_TEST(test_button_falling_edge_detected_once);
    RUN_TEST(test_button_bounce_does_not_trigger_press);
    RUN_TEST(test_button_rising_edge_after_release);
    RUN_TEST(test_button_same_state_no_change);

#ifdef DEVENV
    return UNITY_END();
#else
    UNITY_END();
#endif
}