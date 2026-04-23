/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
//
// #include <stdio.h>
// #include <inttypes.h>
// #include "sdkconfig.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_chip_info.h"
// #include "esp_flash.h"
// #include "esp_system.h"
//
// void app_main(void)
// {
//     printf("Hello world!\n");
//
//     /* Print chip information */
//     esp_chip_info_t chip_info;
//     uint32_t flash_size;
//     esp_chip_info(&chip_info);
//     printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
//            CONFIG_IDF_TARGET,
//            chip_info.cores,
//            (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
//            (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
//            (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
//            (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");
//
//     unsigned major_rev = chip_info.revision / 100;
//     unsigned minor_rev = chip_info.revision % 100;
//     printf("silicon revision v%d.%d, ", major_rev, minor_rev);
//     if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
//         printf("Get flash size failed");
//         return;
//     }
//
//     printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
//            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
//
//     printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
//
//     for (int i = 10; i >= 0; i--) {
//         printf("Restarting in %d seconds...\n", i);
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
//     printf("Restarting now.\n");
//     fflush(stdout);
//     esp_restart();
// }
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define AIN1  GPIO_NUM_18
#define AIN2  GPIO_NUM_19
#define PWMA  GPIO_NUM_21
#define STBY  GPIO_NUM_5

static void motor_forward(void) {
    gpio_set_level(STBY, 1);
    gpio_set_level(AIN1, 1);
    gpio_set_level(AIN2, 0);
    gpio_set_level(PWMA, 1);   // temporary full on, not PWM yet
}

static void motor_reverse(void) {
    gpio_set_level(STBY, 1);
    gpio_set_level(AIN1, 0);
    gpio_set_level(AIN2, 1);
    gpio_set_level(PWMA, 1);   // temporary full on, not PWM yet
}

static void motor_stop(void) {
    gpio_set_level(PWMA, 0);
    gpio_set_level(AIN1, 0);
    gpio_set_level(AIN2, 0);
}

void app_main(void)
{
    gpio_reset_pin(AIN1);
    gpio_reset_pin(AIN2);
    gpio_reset_pin(PWMA);
    gpio_reset_pin(STBY);

    gpio_set_direction(AIN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(AIN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(PWMA, GPIO_MODE_OUTPUT);
    gpio_set_direction(STBY, GPIO_MODE_OUTPUT);

    while (1) {
        printf("Forward\n");
        motor_forward();
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("Stop\n");
        motor_stop();
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("Reverse\n");
        motor_reverse();
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("Stop\n");
        motor_stop();
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
