#include "bsp.h"
#include <unity.h>
#include <string.h>
#include "terminal.h"

void setUp(void) {}
void tearDown(void)
{
    bsp_set_mode_status(true);
    bsp_set_write_status(true);
    bsp_console_set_status(true);
    TEST_ASSERT_TRUE(terminal_init());
    TEST_ASSERT_EQUAL_STRING("", bsp_console_get_output());
    TEST_ASSERT_EQUAL_INT(TERMINAL_LED_PIN, bsp_get_pin_num());
    TEST_ASSERT_EQUAL_INT(0, bsp_get_pin_state());
    TEST_ASSERT_EQUAL_INT(BSP_GPIO_MODE_OUTPUT, bsp_get_pin_mode());
}

void test_terminal_uninitialized(void)
{
    TEST_ASSERT_FALSE(terminal_run());
    TEST_ASSERT_EQUAL_STRING("", bsp_console_get_output());
    TEST_ASSERT_NOT_EQUAL_INT(TERMINAL_LED_PIN, bsp_get_pin_num());
    TEST_ASSERT_NOT_EQUAL_INT(0, bsp_get_pin_state());
    TEST_ASSERT_NOT_EQUAL_INT(BSP_GPIO_MODE_OUTPUT, bsp_get_pin_mode());

    bsp_set_mode_status(false);
    bsp_set_write_status(false);
    TEST_ASSERT_FALSE(terminal_init());
    TEST_ASSERT_EQUAL_STRING("", bsp_console_get_output());
    TEST_ASSERT_EQUAL_INT(BSP_GPIO_MODE_OUTPUT, bsp_get_pin_mode());

    bsp_set_mode_status(false);
    bsp_set_write_status(true);
    TEST_ASSERT_FALSE(terminal_init());
    TEST_ASSERT_EQUAL_STRING("", bsp_console_get_output());

    bsp_set_mode_status(true);
    bsp_set_write_status(false);
    TEST_ASSERT_FALSE(terminal_init());
    TEST_ASSERT_EQUAL_STRING("", bsp_console_get_output());
    TEST_ASSERT_NOT_EQUAL_INT(0, bsp_get_pin_state());
}

void test_terminal_wait_on_user(void)
{
    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_STRING(TERMINAL_COMMAND_MSG, bsp_console_get_output());

    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_STRING("", bsp_console_get_output());

    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_STRING("", bsp_console_get_output());

    bsp_console_set_input('\n');
    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_STRING("\n", bsp_console_get_output());

    bsp_console_set_status(false);
    TEST_ASSERT_FALSE(terminal_run());
    TEST_ASSERT_EQUAL_STRING("", bsp_console_get_output());
}

void test_terminal_invalid_input(void)
{
    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_STRING(TERMINAL_COMMAND_MSG, bsp_console_get_output());

    bsp_console_set_input('p');
    TEST_ASSERT_EQUAL_STRING("", bsp_console_get_output());

    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_STRING("", bsp_console_get_output());

    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_STRING("", bsp_console_get_output());
}

void test_terminal_turn_led_on(void)
{
    char buffer[BSP_CONSOLE_BUFSIZE];

    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_STRING(TERMINAL_COMMAND_MSG, bsp_console_get_output());

    memset(buffer, 0, sizeof(buffer));
    buffer[0] = TERMINAL_LED_ON;
    bsp_console_set_input(buffer[0]);

    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_INT(1, bsp_get_pin_state());
    TEST_ASSERT_EQUAL_INT(TERMINAL_LED_PIN, bsp_get_pin_num());
    TEST_ASSERT_EQUAL_STRING(strcat(buffer, TERMINAL_DONE_MSG), bsp_console_get_output());

    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_STRING(TERMINAL_COMMAND_MSG, bsp_console_get_output());

    bsp_set_write_status(false);
    bsp_console_set_status(true);
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = TERMINAL_LED_OFF;
    bsp_console_set_input(buffer[0]);

    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_INT(1, bsp_get_pin_state());
    TEST_ASSERT_EQUAL_INT(TERMINAL_LED_PIN, bsp_get_pin_num());
    TEST_ASSERT_EQUAL_STRING(strcat(buffer, TERMINAL_FAILED_MSG), bsp_console_get_output());
}

void test_terminal_turn_led_off(void)
{
    char buffer[BSP_CONSOLE_BUFSIZE];

    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_STRING(TERMINAL_COMMAND_MSG, bsp_console_get_output());

    memset(buffer, 0, sizeof(buffer));
    buffer[0] = TERMINAL_LED_OFF;
    bsp_console_set_input(buffer[0]);

    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_INT(0, bsp_get_pin_state());
    TEST_ASSERT_EQUAL_INT(TERMINAL_LED_PIN, bsp_get_pin_num());
    TEST_ASSERT_EQUAL_STRING(strcat(buffer, TERMINAL_DONE_MSG), bsp_console_get_output());

    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_STRING(TERMINAL_COMMAND_MSG, bsp_console_get_output());

    bsp_set_write_status(false);
    bsp_console_set_status(true);
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = TERMINAL_LED_ON;
    bsp_console_set_input(buffer[0]);

    TEST_ASSERT_TRUE(terminal_run());
    TEST_ASSERT_EQUAL_INT(0, bsp_get_pin_state());
    TEST_ASSERT_EQUAL_INT(TERMINAL_LED_PIN, bsp_get_pin_num());
    TEST_ASSERT_EQUAL_STRING(strcat(buffer, TERMINAL_FAILED_MSG), bsp_console_get_output());
}

#ifdef DEVENV
int main(void)
#else
void app_main(void)
#endif
{
    UNITY_BEGIN();

    RUN_TEST(test_terminal_uninitialized);
    RUN_TEST(test_terminal_wait_on_user);
    RUN_TEST(test_terminal_invalid_input);
    RUN_TEST(test_terminal_turn_led_on);
    RUN_TEST(test_terminal_turn_led_off);

#ifdef DEVENV
    return UNITY_END();
#else
    UNITY_END();
#endif
}