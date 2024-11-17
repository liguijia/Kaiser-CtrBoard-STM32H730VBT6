#include "usart_loopback.h"

// 串口句柄

void USART_Loopback_SendData(uint8_t *data, uint16_t size) {
    HAL_UART_Transmit_IT(&huart1, data, size);
    HAL_UART_Transmit_IT(&huart3, data, size);
}

void USART_Loopback_ReceiveData(uint8_t *data, uint16_t size) {
    HAL_UART_Receive_IT(&huart1, data, size);
    HAL_UART_Transmit_IT(&huart3, data, size);
}