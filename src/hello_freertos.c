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

SemaphoreHandle_t semaphore;

#define FIRST_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define SECOND_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )

// Activities 0 & 1
/*
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define HIGH_TASK_PRIORITY     ( tskIDLE_PRIORITY + 4UL )
#define MEDIUM_TASK_PRIORITY     ( tskIDLE_PRIORITY + 3UL )
#define LOW_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define HIGH_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define MEDIUM_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define LOW_TASK_STACK_SIZE configMINIMAL_STACK_SIZE


void high_task(__unused void *params) {
    hard_assert(cyw43_arch_init() == PICO_OK);
    while (true) {
        printf("High priority task starting\n");
        vTaskDelay(1000);
        printf("High priority task delayed\n");
        xSemaphoreTake(semaphore, portMAX_DELAY);
        printf("High priority took semaphore\n");

        //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        //if (count++ % 11) on = !on;
        //vTaskDelay(500);
    }
}

void medium_task(__unused void *params) {
    //hard_assert(cyw43_arch_init() == PICO_OK);
    while (true) {
        printf("Medium priority task starting\n");
        vTaskDelay(500);
        printf("Medium priority task delayed\n");
        //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        //if (count++ % 11) on = !on;
        //vTaskDelay(500);
    }
}

void low_task(__unused void *params) {
    //hard_assert(cyw43_arch_init() == PICO_OK);
    while (true) {
        printf("Low priority task starting\n");
        xSemaphoreTake(semaphore, portMAX_DELAY);
        printf("Low priority task took semaphore\n");

        //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        //if (count++ % 11) on = !on;
        //vTaskDelay(500);
    }
}


void main_task(__unused void *params) {
    xTaskCreate(high_task, "HighThread",
                HIGH_TASK_STACK_SIZE, NULL, HIGH_TASK_PRIORITY, NULL);
    xTaskCreate(medium_task, "MediumThread",
                MEDIUM_TASK_STACK_SIZE, NULL, MEDIUM_TASK_PRIORITY, NULL);
    xTaskCreate(low_task, "LowThread",
                LOW_TASK_STACK_SIZE, NULL, LOW_TASK_PRIORITY, NULL);

    char c;
    while(c = getchar()) {
        if (c <= 'z' && c >= 'a') putchar(c - 32);
        else if (c >= 'A' && c <= 'Z') putchar(c + 32);
        else putchar(c);
    }
}
*/

void busy_busy(int taskNumber)
{
    for (int i = 0; ; i++)
        printf("%d\n",taskNumber);
}

void busy_yield(void)
{
    for (int i = 0; ; i++) {
        taskYIELD();
    }
}

void first_task(__unused void *params)
{
    //vTaskDelay(1000);
    printf("First task starting\n");
    TickType_t start = xTaskGetTickCount();
    busy_yield();
    TickType_t stop = xTaskGetTickCount();
    printf("first task time: %d\n", stop-start);
}

void second_task(__unused void *params)
{
    //vTaskDelay(1000);
    printf("Second task starting\n");
    TickType_t start = xTaskGetTickCount();
    busy_yield();
    TickType_t stop = xTaskGetTickCount();
    printf("second task time: %d\n", stop-start);
}


int main( void )
{
    stdio_init_all();
    sleep_ms(5000); // Give time for TTY to attach.
    printf("started\n");
    const char *rtos_name;
    rtos_name = "FreeRTOS";
    //semaphore = xSemaphoreCreateBinary();
    semaphore = xSemaphoreCreateMutex();
    TaskHandle_t task;
    xTaskCreate(first_task, "FirstThread",
                configMINIMAL_STACK_SIZE, NULL, FIRST_TASK_PRIORITY, &task);
    xTaskCreate(second_task, "SecondThread",
                configMINIMAL_STACK_SIZE, NULL, SECOND_TASK_PRIORITY, &task);
    vTaskStartScheduler();
    return 0;
}
