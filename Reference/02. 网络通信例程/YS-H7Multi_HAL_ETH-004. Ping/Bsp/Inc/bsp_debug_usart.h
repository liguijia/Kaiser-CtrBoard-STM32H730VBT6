/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2018-09-01
  * 功    能: UART-API
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-H7Multi使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
#ifndef __BSP_DEBUG_USART_H__
#define __BSP_DEBUG_USART_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* 宏定义 --------------------------------------------------------------------*/

/* 串口通信的选择 */
#define	USART1_EN                     	         0
#define	USART2_EN                     	         0
#define	USART3_EN                     	         1
#define	USART6_EN                     	         0
#define	UART4_EN                      	         0
/******************** 选择USART1进行通信 ***********************/
#if USART1_EN == 1

#define DEBUG_USARTx                              USART1
#define DEBUG_USARTx_BAUDRATE                     115200
#define DEBUG_USART_RCC_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define DEBUG_USART_RCC_CLK_DISABLE()             __HAL_RCC_USART1_CLK_DISABLE()

#define DEBUG_USARTx_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define DEBUG_USARTx_Tx_GPIO_PIN                  GPIO_PIN_14
#define DEBUG_USARTx_TxRx_GPIO                    GPIOB
#define DEBUG_USARTx_Rx_GPIO_PIN                  GPIO_PIN_15
#define DEBUG_USARTx_TxRx_GPIO                    GPIOB

#define DEBUG_USARTx_AFx                          GPIO_AF4_USART1

#define DEBUG_USART_IRQn                          USART1_IRQn
#define DEBUG_USART_IRQHandler                    USART1_IRQHandler

#endif

/******************** 选择USART2进行通信 ***********************/
#if USART2_EN == 1

#define DEBUG_USARTx                              USART2
#define DEBUG_USARTx_BAUDRATE                     115200
#define DEBUG_USART_RCC_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
#define DEBUG_USART_RCC_CLK_DISABLE()             __HAL_RCC_USART2_CLK_DISABLE()

#define DEBUG_USARTx_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOD_CLK_ENABLE()
#define DEBUG_USARTx_Tx_GPIO_PIN                  GPIO_PIN_5
#define DEBUG_USARTx_TxRx_GPIO                    GPIOD
#define DEBUG_USARTx_Rx_GPIO_PIN                  GPIO_PIN_6
#define DEBUG_USARTx_TxRx_GPIO                    GPIOD

#define DEBUG_USARTx_AFx                          GPIO_AF7_USART2

#define DEBUG_USART_IRQn                          USART2_IRQn
#define DEBUG_USART_IRQHandler                    USART2_IRQHandler

#endif

/******************** 选择USART3进行通信 ***********************/
#if USART3_EN == 1

#define DEBUG_USARTx                              USART3
#define DEBUG_USARTx_BAUDRATE                     115200
#define DEBUG_USART_RCC_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE()
#define DEBUG_USART_RCC_CLK_DISABLE()             __HAL_RCC_USART3_CLK_DISABLE()

#define DEBUG_USARTx_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define DEBUG_USARTx_Tx_GPIO_PIN                  GPIO_PIN_10
#define DEBUG_USARTx_TxRx_GPIO                    GPIOB
#define DEBUG_USARTx_Rx_GPIO_PIN                  GPIO_PIN_11
#define DEBUG_USARTx_TxRx_GPIO                    GPIOB

#define DEBUG_USARTx_AFx                          GPIO_AF7_USART3

#define DEBUG_USART_IRQn                          USART3_IRQn
#define DEBUG_USART_IRQHandler                    USART3_IRQHandler

#endif

/******************** 选择USART6进行通信 ***********************/
#if USART6_EN == 1

#define DEBUG_USARTx                              USART6
#define DEBUG_USARTx_BAUDRATE                     115200
#define DEBUG_USART_RCC_CLK_ENABLE()              __HAL_RCC_USART6_CLK_ENABLE()
#define DEBUG_USART_RCC_CLK_DISABLE()             __HAL_RCC_USART6_CLK_DISABLE()

#define DEBUG_USARTx_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOG_CLK_ENABLE()
#define DEBUG_USARTx_Tx_GPIO_PIN                  GPIO_PIN_14
#define DEBUG_USARTx_TxRx_GPIO                    GPIOG
#define DEBUG_USARTx_Rx_GPIO_PIN                  GPIO_PIN_9
#define DEBUG_USARTx_TxRx_GPIO                    GPIOG

#define DEBUG_USARTx_AFx                          GPIO_AF7_USART6

#define DEBUG_USART_IRQn                          USART6_IRQn
#define DEBUG_USART_IRQHandler                    USART6_IRQHandler

#endif

/******************** 选择UART4进行通信 ***********************/
#if UART4_EN == 1

#define DEBUG_USARTx                              UART4
#define DEBUG_USARTx_BAUDRATE                     115200
#define DEBUG_USART_RCC_CLK_ENABLE()              __HAL_RCC_UART4_CLK_ENABLE()
#define DEBUG_USART_RCC_CLK_DISABLE()             __HAL_RCC_UART4_CLK_DISABLE()

#define DEBUG_USARTx_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define DEBUG_USARTx_Tx_GPIO_PIN                  GPIO_PIN_10
#define DEBUG_USARTx_TxRx_GPIO                    GPIOC
#define DEBUG_USARTx_Rx_GPIO_PIN                  GPIO_PIN_11
#define DEBUG_USARTx_TxRx_GPIO                    GPIOC

#define DEBUG_USARTx_AFx                          GPIO_AF8_UART4

#define DEBUG_USART_IRQn                          UART4_IRQn
#define DEBUG_USART_IRQHandler                    UART4_IRQHandler

#endif

/* 扩展变量 ------------------------------------------------------------------*/
extern UART_HandleTypeDef husart_debug;

/* 函数声明 ------------------------------------------------------------------*/
void MX_DEBUG_UART_Init(void);


#endif /* __BSP_DEBUG_USART_H__ */


/********** (C) COPYRIGHT 2019-2030 硬石嵌入式开发团队 *******END OF FILE******/
