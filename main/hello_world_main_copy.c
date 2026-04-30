#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#define AIN1  GPIO_NUM_18
#define AIN2  GPIO_NUM_23
#define PWMA  GPIO_NUM_19

#define PWM_FREQ_HZ     5000
#define PWM_RESOLUTION  LEDC_TIMER_10_BIT
#define PWM_MAX_DUTY    1023

static void set_motor(int speed)
{
    if (speed > 0) {
        gpio_set_level(AIN1, 1);
        gpio_set_level(AIN2, 0);
    }
    else if (speed < 0) {
        gpio_set_level(AIN1, 0);
        gpio_set_level(AIN2, 1);
        speed = -speed;
    }
    else {
        gpio_set_level(AIN1, 0);
        gpio_set_level(AIN2, 0);
    }

    if (speed > PWM_MAX_DUTY) {
        speed = PWM_MAX_DUTY;
    }

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, speed);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

static void pwm_init(void)
{
    ledc_timer_config_t pwm_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = PWM_RESOLUTION,
        .freq_hz = PWM_FREQ_HZ,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&pwm_timer);

    ledc_channel_config_t pwm_channel = {
        .gpio_num = PWMA,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&pwm_channel);
}

void app_main(void)
{
    gpio_reset_pin(AIN1);
    gpio_reset_pin(AIN2);

    gpio_set_direction(AIN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(AIN2, GPIO_MODE_OUTPUT);

    pwm_init();

    while (1) {
        printf("Forward slow\n");
        set_motor(300);
        vTaskDelay(pdMS_TO_TICKS(6000));

        printf("Stop\n");
        set_motor(0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        printf("Reverse slow\n");
        set_motor(-300);
        vTaskDelay(pdMS_TO_TICKS(6000));

        printf("Stop\n");
        set_motor(0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}