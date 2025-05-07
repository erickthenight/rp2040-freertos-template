#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"

// PINOS
#define LED_PIN 12
#define BUZZER_PIN 10
#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_C 7

bool led_enabled = true;

// === Inicializações ===

void init_output(uint pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
}

void init_button(uint pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

void beep() {
    gpio_put(BUZZER_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_put(BUZZER_PIN, 0);
}

// === TASK LED ===
void led_task(void *params) {
    init_output(LED_PIN);
    while (true) {
        if (led_enabled) {
            gpio_put(LED_PIN, 1);
            vTaskDelay(pdMS_TO_TICKS(200));
            gpio_put(LED_PIN, 0);
            vTaskDelay(pdMS_TO_TICKS(200));
        } else {
            gpio_put(LED_PIN, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

// === TASK BOTÕES ===
void button_task(void *params) {
    init_button(BUTTON_A);
    init_button(BUTTON_B);
    init_button(BUTTON_C);
    init_output(BUZZER_PIN);

    while (true) {
        if (!gpio_get(BUTTON_A)) {
            beep();
            led_enabled = !led_enabled;
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        if (!gpio_get(BUTTON_B)) {
            beep();
            led_enabled = false;
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        if (!gpio_get(BUTTON_C)) {
            beep();
            led_enabled = true;
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// === MAIN ===
int main() {
    stdio_init_all();

    // Criação das tarefas
    xTaskCreate(led_task, "LED Task", 256, NULL, 1, NULL);
    xTaskCreate(button_task, "Button Task", 512, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {}
    return 0;
}