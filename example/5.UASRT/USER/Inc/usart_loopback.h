#ifndef USART_LOOPBACK_H
#define USART_LOOPBACK_H

#include "stm32h7xx_hal.h"
#include "usart.h"

// 发送数据
void USART_Loopback_SendData(uint8_t *data, uint16_t size);

// 接收数据
void USART_Loopback_ReceiveData(uint8_t *data, uint16_t size);

#endif // USART_LOOPBACK_H