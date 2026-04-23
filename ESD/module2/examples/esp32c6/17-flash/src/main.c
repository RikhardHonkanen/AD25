/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using Non-Volatile Storage Library.
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/storage/nvs_flash.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_task_wdt.h"

#define NAME_MAX_LEN 16
#define NAMESPACE "info"
#define KEY "name"

static void clear_input(void)
{
    while (EOF != getchar())
    {
        ;
    }
}

static char read_command(void)
{
    clear_input();

    int command = 0;
    while ((command != 'S') && (command != 'P') && (command != 'R') && (command != '\n'))
    {
        command = toupper(getchar());
    }

    printf("%c", command);

    return command;
}

static void read_string(char *str, int max_len)
{
    int i = 0;
    int chr = 0;

    clear_input();

    while (i < max_len)
    {
        chr = getchar();

        if (chr != EOF)
        {
            if (chr == '\n')
            {
                break;
            }
            else
            {
                printf("%c", chr);
                str[i] = chr;
                i++;
            }
        }
    }

    printf("\n");
}

static void nvs_init(void)
{
    esp_err_t status = nvs_flash_init();
    if ((status == ESP_ERR_NVS_NO_FREE_PAGES) || (status == ESP_ERR_NVS_NEW_VERSION_FOUND))
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        status = nvs_flash_init();
    }
    ESP_ERROR_CHECK(status);
}

static void nvs_write(const char *key, const char *str)
{
    assert((key != NULL) && (str != NULL));

    nvs_handle_t handle = 0;

    ESP_ERROR_CHECK(esp_task_wdt_add(xTaskGetIdleTaskHandle()));

    ESP_ERROR_CHECK(nvs_open(NAMESPACE, NVS_READWRITE, &handle));
    if (str[0] == 0)
    {
        ESP_ERROR_CHECK(nvs_erase_key(handle, key));
    }
    else
    {
        ESP_ERROR_CHECK(nvs_set_str(handle, key, str));
    }
    ESP_ERROR_CHECK(nvs_commit(handle));
    nvs_close(handle);

    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));
}

static bool nvs_read(const char *key, char *str, size_t size)
{
    assert((key != NULL) && (str != NULL) && (size > 1));

    size_t len;
    bool status = false;
    nvs_handle_t handle = 0;

    if (ESP_OK != nvs_open(NAMESPACE, NVS_READONLY, &handle))
    {
        ESP_ERROR_CHECK(nvs_open(NAMESPACE, NVS_READWRITE, &handle));
        nvs_close(handle);
        ESP_ERROR_CHECK(nvs_open(NAMESPACE, NVS_READONLY, &handle));
    }

    if (ESP_OK == nvs_get_str(handle, key, NULL, &len)) // To get the string length
    {
        status = true;
        assert(len <= size);
        ESP_ERROR_CHECK(nvs_get_str(handle, key, str, &len));
    }
    nvs_close(handle);

    return status;
}

void app_main(void)
{
    // Exclude the Idle Task from the Task WDT
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));

    nvs_init(); // Initialize NVS

    while (1)
    {
        printf("S) Set Name\n"
               "P) Print Name\n"
               "R) Reset the µC\n"
               "Enter A Command: ");

        switch (read_command())
        {
        case 'S':
        {
            printf("\nEnter Name: ");

            char buffer[NAME_MAX_LEN + 1] = {0};
            read_string(buffer, NAME_MAX_LEN);

            nvs_write(KEY, buffer);

            if (buffer[0] == 0)
            {
                printf("Name Deleted\n\n");
            }
            else
            {
                printf("Name Stored\n\n");
            }
        }
        break;

        case 'P':
        {
            char buffer[NAME_MAX_LEN + 1] = {0};

            if (nvs_read(KEY, buffer, sizeof(buffer)))
            {
                printf("\nName: %s\n\n", buffer);
            }
            else
            {
                printf("\nName Not Found!\n\n");
            }
        }
        break;

        case 'R':
            esp_restart();
            break;

        default:
            printf("\n");
            break;
        }
    }
}