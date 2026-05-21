/**
 * @file main.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a program for ESP32C6 to get a LED state from terminal and then send it to MCXN236
 via I2C to change state of the onboard LEDs on MCXN236 and get a confimation response from MCXN236
 (ESP32C6 is the I2C master and MCXN236 is the I2C slave).

    If the user input is not valid, ESP32 shall print => invalid to the terminal.

    If the command is confirmed, ESP32 shall print => done to the terminal.

    If the command isn’t confirmed, ESP32 shall print => failed to the terminal.
 * @version 0.1
 * @date 2026-05-21
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <ctype.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/__assert.h>

#define MSG_SIZE 8
#define I2C_NODE DT_PROP(DT_PATH(zephyr_user), i2c)
#define ADDRESS DT_PROP(DT_PATH(zephyr_user), address)
#define RED_LED_NODE DT_ALIAS(led0)
#define GREEN_LED_NODE DT_ALIAS(led1)
#define BLUE_LED_NODE DT_ALIAS(led2)
const struct gpio_dt_spec red_led = GPIO_DT_SPEC_GET(RED_LED_NODE, gpios);
const struct gpio_dt_spec green_led = GPIO_DT_SPEC_GET(GREEN_LED_NODE, gpios);
const struct gpio_dt_spec blue_led = GPIO_DT_SPEC_GET(BLUE_LED_NODE, gpios);

static int count = 0;
static uint8_t buffer[MSG_SIZE];
static uint8_t status;

enum
{
    OFF,
    RED,
    GREEN,
    BLUE
};

enum
{
    SUCCESS,
    FAIL
};

/*
 * @brief Callback which is called when the master starts a write operation.
 * @param config Pointer to the target configuration.
 */
static int on_write_requested(struct i2c_target_config *config) { return 0; }

/*
 * @brief Callback which is called for each byte received from the master.
 * @param config Pointer to the target configuration.
 * @param val The byte received from the master.
 */
static int on_write_received(struct i2c_target_config *config, uint8_t val)
{
    // printk("Received %d\n", val);

    status = SUCCESS;
    buffer[count] = val;
    count = (count + 1) % MSG_SIZE;

    if (0 != gpio_pin_set_dt(&red_led, 0))
    {
        status = FAIL;
    }
    if (0 != gpio_pin_set_dt(&green_led, 0))
    {
        status = FAIL;
    }
    if (0 != gpio_pin_set_dt(&blue_led, 0))
    {
        status = FAIL;
    }
    if (val == RED)
    {
        if (0 != gpio_pin_toggle_dt(&red_led))
        {
            status = FAIL;
        }
    }
    else if (val == GREEN)
    {
        if (0 != gpio_pin_toggle_dt(&green_led))
        {
            status = FAIL;
        }
    }
    else if (val == BLUE)
    {
        if (0 != gpio_pin_toggle_dt(&blue_led))
        {
            status = FAIL;
        }
    }

    return 0;
}

/*
 * @brief Callback which is called when the master starts a read operation.
 * @param config Pointer to the target configuration.
 * @param val Pointer to the byte to be sent to the master.
 */
static int on_read_requested(struct i2c_target_config *config, uint8_t *val)
{
    // *val = buffer[count];
    // count = (count + 1) % MSG_SIZE;
    *val = status;

    return 0;
}

/*
 * @brief Callback which is called after the master reads a byte and requests the next one.
 * @param config Pointer to the target configuration.
 * @param val Pointer to the next byte to be sent to the master.
 */
static int on_read_processed(struct i2c_target_config *config, uint8_t *val)
{
    // *val = buffer[count];
    // count = (count + 1) % MSG_SIZE;
    *val = status;

    return 0;
}

/*
 * @brief Callback which is called when the master sends a stop condition.
 * @param config Pointer to the target configuration.
 */
static int on_stop(struct i2c_target_config *config)
{
    // for (int i = 0; i < MSG_SIZE; i++)
    // {
    //     buffer[i] = toupper(buffer[i]);
    // }

    return 0;
}

int main(void)
{

    const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);

    if (!device_is_ready(i2c_dev))
    {
        printk("I2C device not found!\n");
        exit(EXIT_FAILURE);
    }

    const struct i2c_target_callbacks callbacks = {
        .write_requested = on_write_requested,
        .read_requested = on_read_requested,
        .write_received = on_write_received,
        .read_processed = on_read_processed,
        .stop = on_stop,
    };

    struct i2c_target_config config = {
        .address = ADDRESS,
        .callbacks = &callbacks,
    };

    if (0 != i2c_target_register(i2c_dev, &config))
    {
        printk("I2C target register failed\n");
        exit(EXIT_FAILURE);
    }

    __ASSERT(gpio_is_ready_dt(&red_led), "GPIO port is not ready!");
    __ASSERT(0 == gpio_pin_configure_dt(&red_led, GPIO_OUTPUT_INACTIVE),
             "Failed to configure the gpio pin");
    __ASSERT(gpio_is_ready_dt(&green_led), "GPIO port is not ready!");
    __ASSERT(0 == gpio_pin_configure_dt(&green_led, GPIO_OUTPUT_INACTIVE),
             "Failed to configure the gpio pin");
    __ASSERT(gpio_is_ready_dt(&blue_led), "GPIO port is not ready!");
    __ASSERT(0 == gpio_pin_configure_dt(&blue_led, GPIO_OUTPUT_INACTIVE),
             "Failed to configure the gpio pin");

    while (1)
    {
        k_sleep(K_FOREVER);
    }

    return 0;
}
