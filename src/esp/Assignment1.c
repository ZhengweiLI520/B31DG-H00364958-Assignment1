#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define A 1200
#define B 900
#define C 13
#define D 4500

#define SignalA 2
#define SignalB 3
#define Button1 18
#define Button2 19
#define LedA 2
#define LedB 3
#define OUTPUT_ENABLE 18
#define OUTPUT_SELECT 19

int signalA_enabled = 0;
int outputSelect_enabled = 0;

int last_switch1_state = 0;
int last_switch2_state = 0;

void app_main(void) {
    gpio_set_direction(SignalA, GPIO_MODE_OUTPUT);
    gpio_set_direction(SignalB, GPIO_MODE_OUTPUT);
    gpio_set_direction(Button1, GPIO_MODE_INPUT);
    gpio_set_direction(Button2, GPIO_MODE_INPUT);
    gpio_set_direction(LedA, GPIO_MODE_OUTPUT);
    gpio_set_direction(LedB, GPIO_MODE_OUTPUT);
    gpio_set_direction(OUTPUT_ENABLE, GPIO_MODE_OUTPUT);
    gpio_set_direction(OUTPUT_SELECT, GPIO_MODE_OUTPUT);
    gpio_set_level(OUTPUT_ENABLE, 1); // Enable OUTPUT
    gpio_set_level(OUTPUT_SELECT, 0); // Set OUTPUT_SELECT low initially

    while (1) {
        bool switch1 = gpio_get_level(Button1);
        bool switch2 = gpio_get_level(Button2);

        if (switch1 == 1 && last_switch1_state == 0) {
            signalA_enabled = !signalA_enabled;
        }

        last_switch1_state = switch1;

        if (switch2 != last_switch2_state) {
            outputSelect_enabled = !outputSelect_enabled;
            last_switch2_state = switch2;
            vTaskDelay(50 / portTICK_PERIOD_MS); // Delay to prevent multiple switches
        } else {
            gpio_set_level(OUTPUT_SELECT, outputSelect_enabled);
            last_switch2_state = switch2;
            vTaskDelay(50 / portTICK_PERIOD_MS); // Delay to prevent multiple switches
        }

        if (signalA_enabled) {
            int a = A;
            char i;
            char c;

            if (outputSelect_enabled) {
                c = C - 3;
            } else {
                c = C;
            }

            gpio_set_level(SignalB, 1);
            vTaskDelay(50 / portTICK_PERIOD_MS);
            gpio_set_level(SignalB, 0);

            for (i = 0; i < c; i++) {
                gpio_set_level(SignalA, 1);
                gpio_set_level(LedA, 1);
                vTaskDelay(a / portTICK_PERIOD_MS);
                gpio_set_level(SignalA, 0);
                gpio_set_level(LedA, 0);
                vTaskDelay(B / portTICK_PERIOD_MS);
                a += 50;
            }

            vTaskDelay(D / portTICK_PERIOD_MS);
        } else {
            gpio_set_level(SignalA, 0);
            gpio_set_level(LedA, 0);
        }
    }
}
