#include "bsp.h"
#include "button.h"
#include <unity.h>

#define BUTTON_PIN 4

void setUp(void)
{
}

void tearDown(void) {}

static void test_button_initialization(void)
{
    TEST_ASSERT_EQUAL_INT(BUTTON_UNINITITIALIZED, button_get_state());
    button_update_state();
    TEST_ASSERT_EQUAL_INT(BUTTON_UNINITITIALIZED, button_get_state());

    bsp_set_mode_status(false);
    TEST_ASSERT_FALSE(button_init(BUTTON_PIN));
    TEST_ASSERT_EQUAL_INT(BUTTON_UNINITITIALIZED, button_get_state());
    button_update_state();
    TEST_ASSERT_EQUAL_INT(BUTTON_UNINITITIALIZED, button_get_state());

    bsp_set_mode_status(true);
    TEST_ASSERT_TRUE(button_init(BUTTON_PIN));
    TEST_ASSERT_EQUAL_INT(BUTTON_RELEASED, button_get_state());
    TEST_ASSERT_EQUAL_INT(BUTTON_PIN, bsp_get_pin_num());
    TEST_ASSERT_EQUAL_INT(BSP_MODE_INPUT, bsp_get_pin_mode());
    TEST_ASSERT_EQUAL_INT(BSP_PULL_UP, bsp_get_pin_pull());
}

static void test_button_debounced_state(void)
{
    /////////////////////////////////////////////////////////////////////
    /////////////////// Test the falling edge state /////////////////////
    /////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 4; i++) // 4 times bouncing
    {
        bsp_set_pin_state((i % 2 == 0) ? BSP_LOW : BSP_HIGH);
        button_update_state();
        TEST_ASSERT_EQUAL_INT(BUTTON_RELEASED, button_get_state());
    }

    // The state gets stable
    bsp_set_pin_state(BSP_LOW);
    for (int i = 0; i < BUTTON_SAMPLES - 1; i++)
    {
        button_update_state();
        TEST_ASSERT_EQUAL_INT(BUTTON_RELEASED, button_get_state());
    }
    button_update_state();
    TEST_ASSERT_EQUAL_INT(BUTTON_FALLING_EDGE, button_get_state());

    // The edge was cleared by the previous call to button_get_state
    TEST_ASSERT_EQUAL_INT(BUTTON_PRESSED, button_get_state());

    /////////////////////////////////////////////////////////////////////
    ////////////////////// Test the pressed state ///////////////////////
    /////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 2 * BUTTON_SAMPLES; i++)
    {
        button_update_state();
        TEST_ASSERT_EQUAL_INT(BUTTON_PRESSED, button_get_state());
    }

    /////////////////////////////////////////////////////////////////////
    /////////////////// Test the rising edge state //////////////////////
    /////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 4; i++) // 4 times bouncing
    {
        bsp_set_pin_state((i % 2 == 0) ? BSP_HIGH : BSP_LOW);
        button_update_state();
        TEST_ASSERT_EQUAL_INT(BUTTON_PRESSED, button_get_state());
    }

    // The state gets stable
    bsp_set_pin_state(BSP_HIGH);
    for (int i = 0; i < BUTTON_SAMPLES - 1; i++)
    {
        button_update_state();
        TEST_ASSERT_EQUAL_INT(BUTTON_PRESSED, button_get_state());
    }
    button_update_state();
    TEST_ASSERT_EQUAL_INT(BUTTON_RISING_EDGE, button_get_state());

    // The edge was cleared by the previous call to button_get_state
    TEST_ASSERT_EQUAL_INT(BUTTON_RELEASED, button_get_state());

    /////////////////////////////////////////////////////////////////////
    ////////////////////// Test the released state //////////////////////
    /////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 2 * BUTTON_SAMPLES; i++)
    {
        button_update_state();
        TEST_ASSERT_EQUAL_INT(BUTTON_RELEASED, button_get_state());
    }
}

#ifdef DEVENV
int main(void)
#else
void app_main(void)
#endif
{
    UNITY_BEGIN();

    RUN_TEST(test_button_initialization);
    RUN_TEST(test_button_debounced_state);

#ifdef DEVENV
    return UNITY_END();
#else
    UNITY_END();
#endif
}