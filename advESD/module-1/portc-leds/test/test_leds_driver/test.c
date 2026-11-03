#include "unity.h"
#include "leds_driver.h"

#ifndef DEVENV
#include "portc.h"
#include <Arduino.h>
#endif

static volatile uint8_t *leds_address = NULL;

void setUp(void)
{
    *leds_address = LEDS_DRIVER_ALL_ON;
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_OK, leds_driver_begin(leds_address));
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ALL_OFF, *leds_address);
}

void tearDown(void)
{
    leds_driver_end();
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ALL_ON, *leds_address);
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ERROR, leds_driver_turn_all_off());
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ERROR, leds_driver_turn_all_on());
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ERROR, leds_driver_turn_off(1));
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ERROR, leds_driver_turn_on(1));
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ERROR, leds_driver_get_state(1));
    leds_driver_end();
}

void test_invalid_initialization()
{
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ERROR, leds_driver_begin(NULL));
}

void test_valid_led_number(void)
{
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_OFF, leds_driver_get_state(5));

    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_OK, leds_driver_turn_on(5));
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ON, leds_driver_get_state(5));

#ifndef DEVENV
    delay(1000);
#endif

    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_OK, leds_driver_turn_off(5));
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_OFF, leds_driver_get_state(5));

#ifndef DEVENV
    delay(1000);
#endif
}

void test_invalid_led_number(void)
{
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ERROR, leds_driver_get_state(20));
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ERROR, leds_driver_turn_on(20));
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ERROR, leds_driver_turn_off(20));
}

void test_turn_all_on(void)
{
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_OK, leds_driver_turn_all_on());
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ALL_ON, *leds_address);
}

void test_turn_all_off(void)
{
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_OK, leds_driver_turn_all_off());
    TEST_ASSERT_EQUAL_UINT8(LEDS_DRIVER_ALL_OFF, *leds_address);
}

int main(void)
{
#ifdef DEVENV
    volatile uint8_t virtual_leds;
    leds_address = &virtual_leds;
#else
    portc_init();
    leds_address = portc_address();
#endif

    UNITY_BEGIN();

    RUN_TEST(test_invalid_initialization);
    RUN_TEST(test_valid_led_number);
    RUN_TEST(test_invalid_led_number);
    RUN_TEST(test_turn_all_on);
    RUN_TEST(test_turn_all_off);

    return UNITY_END();
}