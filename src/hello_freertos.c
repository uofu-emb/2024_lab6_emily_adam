/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include <semphr.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

int count = 0;
bool on = false;

#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define HIGH_TASK_PRIORITY     ( tskIDLE_PRIORITY + 3UL )
#define LOW_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define HIGH_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define LOW_TASK_STACK_SIZE configMINIMAL_STACK_SIZE



void high_task(__unused void *params) {
    hard_assert(cyw43_arch_init() == PICO_OK);
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        if (count++ % 11) on = !on;
        vTaskDelay(500);
    }
}

void low_task(__unused void *params) {
    hard_assert(cyw43_arch_init() == PICO_OK);
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        if (count++ % 11) on = !on;
        vTaskDelay(500);
    }
}

void main_task(__unused void *params) {
    xTaskCreate(high_task, "HighThread",
                HIGH_TASK_STACK_SIZE, NULL, HIGH_TASK_PRIORITY, NULL);
    xTaskCreate(low_task, "LowThread",
                LOW_TASK_STACK_SIZE, NULL, LOW_TASK_PRIORITY, NULL);

    char c;
    while(c = getchar()) {
        if (c <= 'z' && c >= 'a') putchar(c - 32);
        else if (c >= 'A' && c <= 'Z') putchar(c + 32);
        else putchar(c);
    }
}

int main( void )
{
    stdio_init_all();
    const char *rtos_name;
    rtos_name = "FreeRTOS";
    SemaphoreHandle_t xSemaphore;
    semaphore = xSemaphoreCreateBoolean();
    TaskHandle_t task;
    xTaskCreate(main_task, "MainThread",
                MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task);
    vTaskStartScheduler();
    return 0;
}
