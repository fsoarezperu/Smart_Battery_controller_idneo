#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/adc.h"

#define TX_PIN (UART_PIN_NO_CHANGE)
#define RX_PIN (UART_PIN_NO_CHANGE)
#define POTENTIOMETER_CHANNEL ADC1_CHANNEL_6

void serialTask(void *parameter) {
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0);

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(POTENTIOMETER_CHANNEL, ADC_ATTEN_DB_11);

    while (1) {
        int rawValue = adc1_get_raw(POTENTIOMETER_CHANNEL);
        float voltage = rawValue * 3.3 / 4095.0; // Convert raw value to voltage
       // printf("Potentiometer Value: %d | Voltage: %.2fV\n", rawValue, voltage);
        printf("Battery Voltage: %.2fV\n", voltage);
        
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main() {
    xTaskCreate(serialTask, "serial_task", 2048, NULL, 10, NULL);
}
